/*	                     __          _
 *	   __  ___________ _/ /___ ___  (_)
 *	  / / / / ___/ __ `/ / __ `__ \/ /
 *	 / /_/ (__  ) /_/ / / / / / / / / 
 *	 \__, /____/\__,_/_/_/ /_/ /_/_/ 
 *	/____/	User: Youssef Salmi
 *			File: ResponsException.cpp 
 */

#include "ResponseException.hpp"

ResponseException::ResponseException() : mMessage("")
{}

ResponseException::ResponseException( const std::string& aMessage ) : mMessage(aMessage)
{}

ResponseException::~ResponseException() throw()
{}

const char*	ResponseException::what() const throw()
{
	return (mMessage.c_str());
}

