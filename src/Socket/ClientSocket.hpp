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

#include "Logger.hpp"

class ClientSocket : public IClientSocket
{
	const int	mID;
	std::string	mBuffer;
    long int    mRead;

	ClientSocket&	operator=( const ClientSocket& c ); // can't be assigned
	ClientSocket( const ClientSocket& aClientSocket);

public:
	ClientSocket(int aFileDes);
	~ClientSocket();

    virtual int         getSocketFd() const;


    virtual int         write( const std::string& aBuffer ) const;
    virtual int         writeAll( const std::string& aBuffer ) const;

    virtual std::string read(unsigned int aMaxSize);
    virtual std::string readTo(const std::string& s);
    virtual std::string readHeaderOnly();
    virtual std::string readAll();

    virtual void        setNonBlocking();

    virtual void        dump();

};
#endif
