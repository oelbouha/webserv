/*	                     __          _
 *	   __  ___________ _/ /___ ___  (_)
 *	  / / / / ___/ __ `/ / __ `__ \/ /
 *	 / /_/ (__  ) /_/ / / / / / / / / 
 *	 \__, /____/\__,_/_/_/ /_/ /_/_/ 
 *	/____/	User: Youssef Salmi
 *			File: ServerSocket.hpp 
 */

#pragma once
#ifndef SERVERSOCKET_HPP
#define SERVERSOCKET_HPP

#include <iostream>

#include <sys/socket.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netinet/in.h>

#include "IServerSocket.hpp"
#include "SocketException.hpp"

#include "ClientSocket.hpp"

#include "Utils.hpp"

class ServerSocket : public IServerSocket
{
	int				mID;
	unsigned int	mIP;
	unsigned int	mPort;

	static int		sBackLog;

public:
	ServerSocket();
	ServerSocket( unsigned long int aIP, unsigned int aPort );
	ServerSocket( const ServerSocket& s);
	~ServerSocket();

	ServerSocket&	operator=( const ServerSocket& s );

	virtual void        	bind();
    virtual void        	listen();
    virtual IClientSocket*	accept();
	virtual void            close();
    virtual void            setNonBlocking();

    virtual int             getPort() const;
    virtual ip_t            getIP() const;

};
#endif
