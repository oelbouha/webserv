/*	                     __          _
 *	   __  ___________ _/ /___ ___  (_)
 *	  / / / / ___/ __ `/ / __ `__ \/ /
 *	 / /_/ (__  ) /_/ / / / / / / / / 
 *	 \__, /____/\__,_/_/_/ /_/ /_/_/ 
 *	/____/	User: Youssef Salmi
 *			File: CGIResponse.cpp 
 */

#include "CGIResponse.hpp"

CGIResponse::CGIResponse(int fd, const IClientSocket& sock):
	mInputFd(fd),
	mSocket(sock),
	mEof(false),
	mFile(-1),
	mHeaderComplete(false),
	mSent(0),
	mWriter(NULL)
{}

CGIResponse::CGIResponse( const CGIResponse& p ):
	mSocket(p.mSocket)
{
	(void)p;
}

CGIResponse::~CGIResponse()
{
	::close(mInputFd);
	::close(mFile);
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

void	CGIResponse::read()
{
	size_t	size = 4096;
	char	buffer[size];
	int		r;

	if (!mHeaderComplete)
	{
		while ((r = ::read(mInputFd, buffer, size)) > 0)
			mHeader += std::string(buffer, r);

		size_t	pos = mHeader.find("\r\n\r\n");
		int		len = 4;
		if (pos == std::string::npos)
		{
			pos = mHeader.find("\n\n");
			len = 2;
		}

		if (pos != std::string::npos)
		{
			std::string	body = mHeader.substr(pos + len);
			mHeader.erase(pos);
			mHeaderComplete = true;

			parseHeader();
			
			bool	location = mResponseHeaders.find("location") != mResponseHeaders.end();
			string_string_map::iterator it = mResponseHeaders.find("content-length");
			// TODO: location with body (chunked)
			if (location || it != mResponseHeaders.end()) {
				if (!location)
					mWriter = new DefaultWriter(mSocket, utils::string_to_uint(it->second));
				else
					mWriter = new DefaultWriter(mSocket, 0);
			} else {
				mWriter = new ChunkedWriter(mSocket);
				mResponseHeaders["transfer-encoding"] = "chunked";
			}
			build();
			
			mWriter->setHeader(mHeader);
			mWriter->append(body);
		
			if (r == 0) {
				mEof = true;
				mWriter->append("");
			}
		}
		return;
	}

	while ((r = ::read(mInputFd, buffer, size)) > 0)
		mWriter->append(std::string(buffer, r));

	if (r == 0) {
		mEof = true;
		mWriter->append("");
	}
}

void	CGIResponse::build()
{
	//	if location
	string_string_map::iterator it = mResponseHeaders.find("location");
	if (it != mResponseHeaders.end()) {
		if (mResponseHeaders.size() > 1) {/*err*/}
		mHeader = "HTTP/1.1 302 Found\r\ncontent-length: 0\r\n";
		mHeader += it->first + ": " + it->second + "\r\n\r\n";
		return;
	}

	//	else 
	//	if no content type supplied
	it = mResponseHeaders.find("content-type");
	if (it != mResponseHeaders.end()) {
		// error
	}

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
	mHeader += "\r\n";
}



void	CGIResponse::send()
{
	// if (mBuffer.empty())
	// 	return;
	// int	r = mSocket.write(mBuffer);
	// mBuffer.erase(0, r);
	if (mWriter)
		mSent += mWriter->write();
	// mSent += r;
}

bool	CGIResponse::sent() const
{
	return (mSent);
}

bool	CGIResponse::done() const
{
	// return (mHeaderComplete && mEof && mBuffer.empty());
	return (mHeaderComplete && mEof && mWriter->done());
}

bool	CGIResponse::error() const
{
	return (mHeaderComplete == false && mEof);
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
		if (pos == std::string::npos)
		{
			// error
		}
		key = line.substr(0, pos);
		value = line.substr(pos + 1);

		utils::str_to_lower(utils::trim_spaces(key));
		utils::trim_spaces(value);

		if (key.empty() || value.empty())
		{
			// error;
		}

		// if element already exist

		mResponseHeaders[key] = value;
	}
}
