/*	                     __          _
 *	   __  ___________ _/ /___ ___  (_)
 *	  / / / / ___/ __ `/ / __ `__ \/ /
 *	 / /_/ (__  ) /_/ / / / / / / / / 
 *	 \__, /____/\__,_/_/_/ /_/ /_/_/ 
 *	/____/	User: Youssef Salmi
 *			File: DescriptorProxyRequest.hpp 
 */

#pragma once
#ifndef DESCRIPTORPROXYREQUEST_HPP
#define DESCRIPTORPROXYREQUEST_HPP

#include <iostream>
#include <unistd.h>

#include "src/Interfaces/IRequest.hpp"

#include "IProxyRequest.hpp"

class DescriptorProxyRequest : public IProxyRequest
{
    int             mOutputFd;
    IRequest&       mRequest; 
    std::string     mBuffer;

	DescriptorProxyRequest( const DescriptorProxyRequest& d);
	DescriptorProxyRequest&	operator=( const DescriptorProxyRequest& d );

public:
	DescriptorProxyRequest(int fd, IRequest& req);
	~DescriptorProxyRequest();

    int     getOutputFd() const;
    int     getSocketFd() const;

    void    read();
    void    send();
    bool    done() const;
};
#endif
