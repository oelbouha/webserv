/*	                     __          _
 *	   __  ___________ _/ /___ ___  (_)
 *	  / / / / ___/ __ `/ / __ `__ \/ /
 *	 / /_/ (__  ) /_/ / / / / / / / / 
 *	 \__, /____/\__,_/_/_/ /_/ /_/_/ 
 *	/____/	User: Youssef Salmi
 *			File: BufferProxyRequest.hpp 
 */

#pragma once
#ifndef BUFFERPROXYREQUEST_HPP
#define BUFFERPROXYREQUEST_HPP


#include <string>

#include <unistd.h>

#include "IProxyRequest.hpp"

class BufferProxyRequest : public IProxyRequest
{
    int             mOutputFd;
    std::string     mBuffer;
    size_t          mCursor;

	BufferProxyRequest&	operator=( const BufferProxyRequest& b );
	BufferProxyRequest( const BufferProxyRequest& b);

public:
    BufferProxyRequest(int fd, const std::string& buffer);
	~BufferProxyRequest();

    int     getOutputFd() const;
    int     getSocketFd() const;

    void    read();
    void    send();
    bool    done() const;

};
#endif
