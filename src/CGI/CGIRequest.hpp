/*	                     __          _
 *	   __  ___________ _/ /___ ___  (_)
 *	  / / / / ___/ __ `/ / __ `__ \/ /
 *	 / /_/ (__  ) /_/ / / / / / / / / 
 *	 \__, /____/\__,_/_/_/ /_/ /_/_/ 
 *	/____/	User: Youssef Salmi
 *			File: CGIRequest.hpp 
 */

#pragma once
#ifndef CGIRequest_HPP
#define CGIRequest_HPP

#include <iostream>
#include <unistd.h>

#include "src/Interfaces/IClient.hpp"

#include "src/Request/Request.hpp"
#include "src/Server/ErrorPages.hpp"

#include "IProxyRequest.hpp"

class CGIRequest : public IProxyRequest
{
    int             mOutputFd;
    Request&        mRequest; 
    std::string     mBuffer;
    const ErrorPages* mErrorPages;


public:
    IClient*        client;

public:

	CGIRequest( const CGIRequest& d);
	CGIRequest&	operator=( const CGIRequest& d );

public:
	CGIRequest(int fd, Request& req);
	~CGIRequest();

    int     getOutputFd() const;
    int     getSocketFd() const;
    void    setErrorPages( const ErrorPages* error_pages );

    IResponse*  buildErrorPage(int code) const;

    void    read();
    void    send();
    bool    done() const;
};
#endif
