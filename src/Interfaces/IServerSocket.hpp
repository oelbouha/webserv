/*	                     __          _
 *	   __  ___________ _/ /___ ___  (_)
 *	  / / / / ___/ __ `/ / __ `__ \/ /
 *	 / /_/ (__  ) /_/ / / / / / / / / 
 *	 \__, /____/\__,_/_/_/ /_/ /_/_/ 
 *	/____/	User: Youssef Salmi
 *			File: IServerSocket.hpp 
 */

#pragma once
#ifndef ISERVERSOCKET_HPP
#define ISERVERSOCKET_HPP

#include "src/Interfaces/IClientSocket.hpp"

#include "include/Types.hpp"

class IServerSocket
{
public:
	virtual		~IServerSocket(){};
	virtual void        	bind() = 0;
    virtual void        	listen() = 0;
    virtual IClientSocket*	accept() const = 0;
    virtual void            setNonBlocking() = 0;

    virtual int             getPort() const = 0;
    virtual ip_t            getIP() const = 0;

    virtual int             getSocketFd() const = 0;

};
#endif
