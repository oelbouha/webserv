/*	                     __          _
 *	   __  ___________ _/ /___ ___  (_)
 *	  / / / / ___/ __ `/ / __ `__ \/ /
 *	 / /_/ (__  ) /_/ / / / / / / / / 
 *	 \__, /____/\__,_/_/_/ /_/ /_/_/ 
 *	/____/	User: Youssef Salmi
 *			File: BufferProxyRequest.cpp 
 */

#include "BufferProxyRequest.hpp"

BufferProxyRequest::BufferProxyRequest(int fd, const std::string& buffer) :
    mOutputFd(fd),
    mBuffer(buffer),
    mCursor(0)
{}

BufferProxyRequest::BufferProxyRequest( const BufferProxyRequest& b ) :
    mOutputFd(b.mOutputFd),
    mBuffer(b.mBuffer),
    mCursor(b.mCursor)
{}

BufferProxyRequest::~BufferProxyRequest()
{
    ::close(mOutputFd);
}

BufferProxyRequest&	BufferProxyRequest::operator=( const BufferProxyRequest& b )
{
    if (this != &b)
        mBuffer = b.mBuffer;

    return (*this);
}

int     BufferProxyRequest::getOutputFd() const
{
    return (mOutputFd);

}

int     BufferProxyRequest::getSocketFd() const
{
    return (-1);
}

bool    BufferProxyRequest::done() const
{
    return (mBuffer.length() == mCursor);
}

void    BufferProxyRequest::read()
{
    // does nothing;
}

void    BufferProxyRequest::send()
{
    int r = ::write(mOutputFd,
                    mBuffer.data() + mCursor,
                    mBuffer.length() - mCursor);
    if (r > 0)
        mCursor += r;
}
