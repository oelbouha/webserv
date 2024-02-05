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
	mSent(0)
{
	// std::cout << "cgi response +++++++++++++++++++++++++++++++ opened " << mInputFd << std::endl;
}

CGIResponse::CGIResponse( const CGIResponse& p ):
	mSocket(p.mSocket)
{
	(void)p;
}

CGIResponse::~CGIResponse()
{
	// std::cout << "cgi response +++++++++++++++++++++++++++++++ closing " << mInputFd << std::endl;
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

		if (r == 0)
			mEof = true;
		

		size_t	pos = mHeader.find("\r\n\r\n");
		int		len = 4;
		if (pos == std::string::npos)
		{
			pos = mHeader.find("\n\n");
			len = 2;
		}

		if (pos != std::string::npos)
		{
			mBuffer = mHeader.substr(pos + len);
			mHeader.erase(pos);
			build();
			mHeaderComplete = true;
		}
		return;
	}

	while ((r = ::read(mInputFd, buffer, size)) > 0)
		mBuffer += std::string(buffer, r);
	
	if (r == 0)
		mEof = true;
}

void	CGIResponse::build()
{
	std::string	responseHeader = "HTTP/1.1 200 OK\r\n";

	parseHeader();

	// if location

	// else if no content type supplied
	std::map<std::string, std::string>::iterator it = mResponseHeaders.find("content-type");
	if (it != mResponseHeaders.end())
	{
		// error
	}
	// else
	it = mResponseHeaders.find("status");
	if (it != mResponseHeaders.end())
	{
		responseHeader = "HTTP/1.1 " + it->second + "\r\n";
		mResponseHeaders.erase(it);
	}
	responseHeader += "Content-Length: " + utils::to_string(mBuffer.length()) + "\r\n";
	it = mResponseHeaders.begin();
	while (it != mResponseHeaders.end())
	{
		responseHeader += it->first + ": " + it->second + "\r\n";
		++it;
	}
	// if no content length is supplied choose chunked transfer enconding
	responseHeader += "\r\n";

	mBuffer = responseHeader + mBuffer;
}



void	CGIResponse::send()
{
	if (mBuffer.empty())
		return;
	int	r = mSocket.write(mBuffer);
	std::cout << mBuffer.substr(0, r) << std::endl;
	mBuffer.erase(0, r);
	mSent += r;
}

bool	CGIResponse::sent() const
{
	return (mSent);
}

bool	CGIResponse::done() const
{
	return (mHeaderComplete && mEof && mBuffer.empty());
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
		utils::str_to_lower(utils::trim_spaces(value));

		if (key.empty() || value.empty())
		{
			// error;
		}

		// if element already exist

		mResponseHeaders[key] = value;
	}
}