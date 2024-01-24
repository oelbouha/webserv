/*	                     __          _
 *	   __  ___________ _/ /___ ___  (_)
 *	  / / / / ___/ __ `/ / __ `__ \/ /
 *	 / /_/ (__  ) /_/ / / / / / / / / 
 *	 \__, /____/\__,_/_/_/ /_/ /_/_/ 
 *	/____/	User: Youssef Salmi
 *			File: IClientSocket.hpp 
 */

#pragma once
#ifndef ICLIENTSOCKET_HPP
#define ICLIENTSOCKET_HPP

#include <string>

class IClientSocket
{
public:
	virtual		~IClientSocket(){};

    virtual int         getSocketFd() const = 0;

    virtual int         write( const std::string& aBuffer ) const = 0;
    virtual int         writeAll( const std::string& aBuffer ) const = 0;

    virtual std::string readAll() = 0;
    virtual std::string readHeaderOnly() = 0;
    virtual std::string read( unsigned int aMaxSize ) = 0;

    virtual void        setNonBlocking() = 0;

    virtual void        dump() = 0;
};
#endif
