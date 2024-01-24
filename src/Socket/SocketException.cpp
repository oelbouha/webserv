/*	                     __          _
 *	   __  ___________ _/ /___ ___  (_)
 *	  / / / / ___/ __ `/ / __ `__ \/ /
 *	 / /_/ (__  ) /_/ / / / / / / / / 
 *	 \__, /____/\__,_/_/_/ /_/ /_/_/ 
 *	/____/	User: Youssef Salmi
 *			File: SocketException.cpp 
 */

#include "SocketException.hpp"

SocketException::SocketException( const std::string& aMessage, SocketException::Error aErr):
mMessage(aMessage),
error(aErr)
{}

SocketException::SocketException( SocketException::Error aErr ):
mMessage("Socket Error"),
error(aErr)
{}

SocketException::~SocketException( void ) throw()
{}

const char*	SocketException::what( void ) const throw()
{
	return (mMessage.c_str());
}

