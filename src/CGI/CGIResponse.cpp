/*	                     __          _
 *	   __  ___________ _/ /___ ___  (_)
 *	  / / / / ___/ __ `/ / __ `__ \/ /
 *	 / /_/ (__  ) /_/ / / / / / / / / 
 *	 \__, /____/\__,_/_/_/ /_/ /_/_/ 
 *	/____/	User: Youssef Salmi
 *			File: CGIResponse.cpp 
 */

#include "CGIResponse.hpp"

CGIResponse::CGIResponse(int fd, IRequest* req) :
	mInputFd(fd),
	mSocket(req->getSocket()),
	mEof(false),
	mHeaderComplete(false),
	mError(false),
	mSent(0),
	mWriter(NULL),
	mErrorPages(NULL),
	mLastReadTimeStamp(std::time(0)),
	mLastSendTimeStamp(std::time(0)),
	request(req)
{}

CGIResponse::CGIResponse( const CGIResponse& p ):
	mSocket(p.mSocket)
{
	(void)p;
}

CGIResponse::~CGIResponse()
{
	delete mWriter;
	::close(mInputFd);
}

CGIResponse&	CGIResponse::operator=( const CGIResponse& p )
{
	(void)p;
	return (*this);
}

int		CGIResponse::getInputFd() const
{
	return (mInputFd);
}

int		CGIResponse::getSocketFd() const
{
	return (mSocket.getSocketFd());
}

bool	CGIResponse::hasTimedOut() const
{
	uint read_diff = std::difftime(std::time(0), mLastReadTimeStamp);
	uint send_diff = std::difftime(std::time(0), mLastSendTimeStamp);

	return (read_diff >= CGI_READ_TIMEOUT && send_diff >= CGI_SEND_TIMEOUT);
}

void    CGIResponse::setErrorPages( const ErrorPages* error_pages )
{
    mErrorPages = error_pages;
}

IResponse*  CGIResponse::buildErrorPage(int code) const
{
    return (mErrorPages->build(*request, code));
}

void		CGIResponse::extractHeader()
{
	size_t	pos = mHeader.find("\r\n\r\n");
	int		len = 4;
	if (pos == std::string::npos)
	{
		pos = mHeader.find("\n\n");
		len = 2;
	}
	if (pos != std::string::npos)
	{
		mBody = mHeader.substr(pos + len);
		mHeader.erase(pos);
		mHeaderComplete = true;
	}
}

void	CGIResponse::read()
{
	char	buffer[CGI_BUFFER_SIZE];
	int		r;

	if (mEof) return;

	std::time(&mLastReadTimeStamp);

	if (!mHeaderComplete)
	{
		while ((r = ::read(mInputFd, buffer, CGI_BUFFER_SIZE)) > 0)
			mHeader += std::string(buffer, r);

		if (r == 0) mEof = true;

		extractHeader();
		if (mHeaderComplete)
		{
			Logger::debug ("header completed").flush();
			parseHeader();
			setWriter();
			build();
			
			mWriter->setHeader(mHeader);
			mWriter->append(mBody);
			if (r == 0)  mWriter->append("");
		}
		
		
		return;
	}

	while ((r = ::read(mInputFd, buffer, CGI_BUFFER_SIZE)) > 0)
		mWriter->append(std::string(buffer, r));

	if (r == 0) {
		mEof = true;
		mWriter->append("");
	}
}

void	CGIResponse::setWriter()
{
	string_string_map::iterator cl = mResponseHeaders.find("content-length");

	uint	content_len = 0;
	if (cl != mResponseHeaders.end())
		content_len = utils::string_to_uint(cl->second);

	if (cl != mResponseHeaders.end())
		mWriter = new DefaultWriter(mSocket, content_len);
	else {
		mWriter = new ChunkedWriter(mSocket);
		if (!isLocalRedirection())
			mAdditionalHeader += "Transfer-Encoding: chunked\r\n";
	}
}

void    CGIResponse::setHeader(const std::string& header, const std::string& value)
{
	mAdditionalHeader += header + ": " + value + "\r\n";
}

void	CGIResponse::build()
{
	string_string_map::iterator it;
	string_string_map::iterator end = mResponseHeaders.end();

	//	if location
	it = mResponseHeaders.find("location");
	if (it != end) {
		const std::string& location = it->second;

		if (location[0] == '/') { // local redirect
			if ( mResponseHeaders.size() > 1 ) mError = true;
			else if ( ! mEof || ! mBody.empty() ) mError = true;
		}
		else { // client redirect
			it = mResponseHeaders.find("status");

			if (it == end) { // redirect without a body
				Logger::debug ("redir without body:").flush();
				Logger::debug ("\theaders size: ")(mResponseHeaders.size()).flush();
				Logger::debug ("\tmBody: *")(mBody)("*").flush();
				if ( mResponseHeaders.size() > 1 || ! mBody.empty() ) { mError = true; return; }
				mHeader = "HTTP/1.1 302 Found\r\nContent-Length: 0\r\n";
				it = mResponseHeaders.begin();
				for (; it != mResponseHeaders.end(); ++it)
					mHeader += it->first + ": " + it->second + "\r\n";
			}
			else { // redirect with body
				if ( it == end || it->second[0] != '3' ) { mError = true; return; }
				if ( mResponseHeaders.find("content-type") == end ) { mError = true; return; }

				std::string& status = it->second;
				mHeader = "HTTP/1.1 " + status + "\r\n";
				mResponseHeaders.erase(it);
				it = mResponseHeaders.begin();
				for (; it != mResponseHeaders.end(); ++it)
					mHeader += it->first + ": " + it->second + "\r\n";
			}
			mHeader += mAdditionalHeader + "\r\n";
		}
		return;
	}

	//	else 
	//	if no content type supplied
	it = mResponseHeaders.find("content-type");
	if (it == mResponseHeaders.end()) { mError = true; return; }
	Logger::debug ("content type found").flush();

	// else
	it = mResponseHeaders.find("status");
	if (it != mResponseHeaders.end()) {
		mHeader = "HTTP/1.1 " + it->second + "\r\n";
		mResponseHeaders.erase(it);
	}
	else mHeader = "HTTP/1.1 200 OK\r\n";

	it = mResponseHeaders.begin();
	while (it != mResponseHeaders.end()) {
		mHeader += it->first + ": " + it->second + "\r\n";
		++it;
	}
	mHeader += mAdditionalHeader + "\r\n";
}

bool	CGIResponse::isLocalRedirection() const
{
	if (! mHeaderComplete || mError ) return false;

	string_string_map::const_iterator	it = mResponseHeaders.find("location");

	if (it == mResponseHeaders.end()) return false;

	const std::string& location = it->second;

	return location.at(0) == '/';
}

std::string	CGIResponse::getRedirectionLocation() const
{
	string_string_map::const_iterator it = mResponseHeaders.find("location");

	if (it == mResponseHeaders.end()) return "";

	return it->second;
}

void	CGIResponse::send()
{
	if (error()) return;
	if (mWriter == NULL) return;
	
	int r = mWriter->write();
	if (r == 0) return;
	mSent += r;
	std::time(&mLastSendTimeStamp);
}

bool	CGIResponse::sent() const
{
	return (mSent);
}

bool	CGIResponse::done() const
{
	return (mHeaderComplete && mEof && mWriter->done());
}

bool	CGIResponse::error()
{
	if (! mError && ! mHeaderComplete && mEof ) mError = true;
	
	return mError;
}

void	CGIResponse::parseHeader()
{
	std::istringstream	ss(mHeader);
	std::string			line;
	std::string			key, value;

	while (!ss.eof())
	{
		std::getline(ss, line);

		if (line[line.length() - 1] == '\r')
			line.erase(line.length() - 1);

		size_t	pos = line.find(":");
		if (pos == std::string::npos) {
			mError = true;
			return;
		}
		key = line.substr(0, pos);
		value = line.substr(pos + 1);

		utils::str_to_lower(utils::trim_spaces(key));
		utils::trim_spaces(value);

		if (key.empty() || value.empty()) {
			mError = true;
			return;
		}

		mResponseHeaders[key] = value;
	}
}
