/*	                     __          _
 *	   __  ___________ _/ /___ ___  (_)
 *	  / / / / ___/ __ `/ / __ `__ \/ /
 *	 / /_/ (__  ) /_/ / / / / / / / / 
 *	 \__, /____/\__,_/_/_/ /_/ /_/_/ 
 *	/____/	User: Youssef Salmi
 *			File: SocketException.hpp 
 */

#pragma once
#ifndef SOCKETEXCEPTION_HPP
#define SOCKETEXCEPTION_HPP

#include <string>
#include <cstring>

class SocketException : public std::exception
{
    std::string	mMessage;

public:
    enum Error {
        CANT_BE_SET_TO_NON_BLOCKING,
        CONNECTION_CLOSED,
        CANT_WRITE_TO_SOCKET,
        DEFAULT
    };

    Error   error;

public:
    SocketException( const std::string& aMsg = "Socket Error", Error aErr = DEFAULT );
    SocketException( Error aErr );

    ~SocketException() throw ();
    const char*	what( void ) const throw();


};
#endif
