/*	                     __          _
 *	   __  ___________ _/ /___ ___  (_)
 *	  / / / / ___/ __ `/ / __ `__ \/ /
 *	 / /_/ (__  ) /_/ / / / / / / / / 
 *	 \__, /____/\__,_/_/_/ /_/ /_/_/ 
 *	/____/	User: Youssef Salmi
 *			File: IProxyResponse.hpp 
 */

#pragma once
#ifndef IPROXYRESPONSE_HPP
#define IPROXYRESPONSE_HPP

class IProxyResponse
{
public:
	virtual		~IProxyResponse(){};

    virtual int         getInputFd() const = 0;
    virtual int         getSocketFd() const = 0;

    virtual void        build() = 0;

    virtual void        read() = 0;
    virtual void        send() = 0;
    virtual bool        done() const = 0;
    virtual bool        error() const = 0;
    virtual bool        sent() const = 0;

};
#endif
