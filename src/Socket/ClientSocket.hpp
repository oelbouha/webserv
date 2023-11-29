/*	                     __          _
 *	   __  ___________ _/ /___ ___  (_)
 *	  / / / / ___/ __ `/ / __ `__ \/ /
 *	 / /_/ (__  ) /_/ / / / / / / / / 
 *	 \__, /____/\__,_/_/_/ /_/ /_/_/ 
 *	/____/	User: Youssef Salmi
 *			File: ClientSocket.hpp 
 */

#pragma once
#ifndef CLIENTSOCKET_HPP
#define CLIENTSOCKET_HPP

#include <iostream>
#include <string>
#include <cstring>

#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <netinet/in.h>

#include "IClientSocket.hpp"
#include "SocketException.hpp"

class ClientSocket : public IClientSocket
{
	const int	mID;
	std::string	mBuffer;

	ClientSocket&	operator=( const ClientSocket& c ); // can't be assigned

public:
	ClientSocket(int aFileDes);
	ClientSocket( const ClientSocket& aClientSocket);
	~ClientSocket();


    virtual int         write( const char* aBuffer, int aSize );

    virtual std::string read(unsigned int aMaxSize);
    virtual std::string readHeaderOnly();
    virtual std::string readAll();

	virtual void		close();
    virtual void        setNonBlocking();

    virtual void        dump();

};
#endif
