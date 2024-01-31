/*	                     __          _
 *	   __  ___________ _/ /___ ___  (_)
 *	  / / / / ___/ __ `/ / __ `__ \/ /
 *	 / /_/ (__  ) /_/ / / / / / / / / 
 *	 \__, /____/\__,_/_/_/ /_/ /_/_/ 
 *	/____/	User: Youssef Salmi
 *			File: DescriptorProxyRequest.cpp 
 */

#include "DescriptorProxyRequest.hpp"

DescriptorProxyRequest::DescriptorProxyRequest(int fd, IRequest& req) :
    mOutputFd(fd),
    mRequest(req)
{
    // std::cout << "cgi request    +++++++++++++++++++++++++++++++ opened " << mOutputFd << std::endl;
}

DescriptorProxyRequest::DescriptorProxyRequest( const DescriptorProxyRequest& d )
    : mRequest(d.mRequest)
{
    (void)d;
}

DescriptorProxyRequest::~DescriptorProxyRequest()
{
    // std::cout << "cgi request +++++++++++++++++++++++++++++++ closing " << mOutputFd << std::endl;
    ::close(mOutputFd);
    delete &mRequest;
}

DescriptorProxyRequest&	DescriptorProxyRequest::operator=( const DescriptorProxyRequest& d )
{
    (void)d;
	return (*this);
}

int     DescriptorProxyRequest::getOutputFd() const
{
    return (mOutputFd);
}

int     DescriptorProxyRequest::getSocketFd() const
{
    return (mRequest.getSocket().getSocketFd());
}

bool    DescriptorProxyRequest::done() const
{
    return (mRequest.done() && mBuffer.empty());
}

void    DescriptorProxyRequest::read()
{
    if (!mRequest.done())
        mBuffer += mRequest.read();
}

void    DescriptorProxyRequest::send()
{
    if (mBuffer.empty())
        return;

    int r = ::write(mOutputFd, mBuffer.data(), mBuffer.length());

    if (r > 0)
        mBuffer.erase(0, r);
}

