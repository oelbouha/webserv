/*	                     __          _
 *	   __  ___________ _/ /___ ___  (_)
 *	  / / / / ___/ __ `/ / __ `__ \/ /
 *	 / /_/ (__  ) /_/ / / / / / / / / 
 *	 \__, /____/\__,_/_/_/ /_/ /_/_/ 
 *	/____/	User: Youssef Salmi
 *			File: CGIRequest.cpp 
 */

#include "CGIRequest.hpp"

CGIRequest::CGIRequest(int fd, Request& req) :
    mOutputFd(fd),
    mRequest(req)
{}

CGIRequest::CGIRequest( const CGIRequest& d )
    : mRequest(d.mRequest)
{
    (void)d;
}

CGIRequest::~CGIRequest()
{
    ::close(mOutputFd);
}

CGIRequest&	CGIRequest::operator=( const CGIRequest& d )
{
    (void)d;
	return (*this);
}

int     CGIRequest::getOutputFd() const
{
    return (mOutputFd);
}

int     CGIRequest::getSocketFd() const
{
    return (mRequest.getSocketFd());
}

void    CGIRequest::setErrorPages( const ErrorPages* error_pages )
{
    mErrorPages = error_pages;
}

IResponse*  CGIRequest::buildErrorPage(int code) const
{
    return (mErrorPages->build(mRequest, code));
}

bool    CGIRequest::done() const
{
    return (mRequest.done() && mBuffer.empty());
}

void    CGIRequest::read()
{
    if (!mRequest.done())
        mBuffer += mRequest.read();
    Logger::debug ("cgi request read: ")(mBuffer).flush();
}

void    CGIRequest::send()
{
    if (mBuffer.empty())
        return;

    int r = ::write(mOutputFd, mBuffer.data(), mBuffer.length());

    if (r < 0)
        throw std::invalid_argument("invalid");

    mBuffer.erase(0, r);
}

