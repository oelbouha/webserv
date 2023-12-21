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

#include <iostream>
#include <string>
#include <cstring>

class SocketException : public std::exception
{
	std::string	mMessage;

	public:
		SocketException();
		SocketException( const std::string& aMessage );
		~SocketException() throw ();
		const char*	what( void ) const throw();


};
#endif
