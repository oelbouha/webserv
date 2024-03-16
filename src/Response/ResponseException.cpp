/*	                     __          _
 *	   __  ___________ _/ /___ ___  (_)
 *	  / / / / ___/ __ `/ / __ `__ \/ /
 *	 / /_/ (__  ) /_/ / / / / / / / / 
 *	 \__, /____/\__,_/_/_/ /_/ /_/_/ 
 *	/____/	User: Youssef Salmi
 *			File: ResponsException.cpp 
 */

#include "ResponseException.hpp"

ResponseException::ResponseException() : mMessage("Response Error")
{}

ResponseException::ResponseException( const std::string& msg, ResponseException::Error err ) :
	mMessage(msg),
	error(err)
{}

ResponseException::ResponseException( ResponseException::Error err ) :
	mMessage("Response Error"),
	error(err)
{}

ResponseException::~ResponseException() throw()
{}

const char*	ResponseException::what() const throw()
{
	return (mMessage.c_str());
}

