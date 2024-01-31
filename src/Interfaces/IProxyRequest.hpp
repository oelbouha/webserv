/*	                     __          _
 *	   __  ___________ _/ /___ ___  (_)
 *	  / / / / ___/ __ `/ / __ `__ \/ /
 *	 / /_/ (__  ) /_/ / / / / / / / / 
 *	 \__, /____/\__,_/_/_/ /_/ /_/_/ 
 *	/____/	User: Youssef Salmi
 *			File: IProxyRequest.hpp 
 */

#pragma once
#ifndef IPROXYREQUEST_HPP
#define IPROXYREQUEST_HPP

class IProxyRequest
{
public:
	virtual		~IProxyRequest(){};

    virtual int         getOutputFd() const = 0;
    virtual int         getSocketFd() const = 0;

    virtual void        read() = 0;
    virtual void        send() = 0;
    virtual bool        done() const = 0;

};
#endif
