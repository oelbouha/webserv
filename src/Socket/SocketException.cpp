/*	                     __          _
 *	   __  ___________ _/ /___ ___  (_)
 *	  / / / / ___/ __ `/ / __ `__ \/ /
 *	 / /_/ (__  ) /_/ / / / / / / / / 
 *	 \__, /____/\__,_/_/_/ /_/ /_/_/ 
 *	/____/	User: Youssef Salmi
 *			File: SocketException.cpp 
 */

#include "SocketException.hpp"

SocketException::SocketException( void ) : mMessage("Socket Error")
{}

SocketException::SocketException( const std::string& aMessage ) : mMessage(aMessage)
{}

SocketException::~SocketException( void ) throw()
{}

const char*	SocketException::what( void ) const throw()
{
	return (mMessage.c_str());
}

