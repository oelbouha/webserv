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

#include "src/Interfaces/IRequest.hpp"

#include "IProxyRequest.hpp"

class CGIRequest : public IProxyRequest
{
    int             mOutputFd;
    IRequest&       mRequest; 
    std::string     mBuffer;

	CGIRequest( const CGIRequest& d);
	CGIRequest&	operator=( const CGIRequest& d );

public:
	CGIRequest(int fd, IRequest& req);
	~CGIRequest();

    int     getOutputFd() const;
    int     getSocketFd() const;

    void    read();
    void    send();
    bool    done() const;
};
#endif
