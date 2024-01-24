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
	mEof(false)
{}

CGIResponse::CGIResponse( const CGIResponse& p )
{
	(void)p;
}

CGIResponse::~CGIResponse()
{}

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

void	CGIResponse::readAll()
{
	size_t	size = 4096;
	char	buffer[size];
	int		r;

	while ((r = ::read(mInputFd, buffer, size)) > 0)
		mBuffer += std::string(buffer, r);
	
	if (r == 0)
		mEof = true;
}

void	CGIResponse::build()
{
}

void	CGIResponse::send()
{
	int	r = mSocket.write(mBuffer);
	mBuffer.erase(0, r);
}

bool	CGIResponse::done() const
{
	return (mEof);
}