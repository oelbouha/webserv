/*	                     __          _
 *	   __  ___________ _/ /___ ___  (_)
 *	  / / / / ___/ __ `/ / __ `__ \/ /
 *	 / /_/ (__  ) /_/ / / / / / / / /
 *	 \__, /____/\__,_/_/_/ /_/ /_/_/
 *	/____/	User: Youssef Salmi
 *			File: RequestException.cpp
 */

#include "RequestException.hpp"

RequestException::RequestException() :
    mMessage("Request Exception"),
    error(DEFAULT)
{
    Logger::debug (mMessage).flush();
}

RequestException::RequestException(const std::string &aMessage, RequestException::Error error) :
    mMessage(aMessage),
    error(error)
{
    Logger::debug (aMessage).flush();
}

RequestException::RequestException(RequestException::Error error) :
    mMessage("Request Exception Error"),
    error(error)
{
    Logger::debug (mMessage).flush();
}

RequestException::~RequestException() throw() {}

const char *RequestException::what() const throw()
{
  return (mMessage.c_str());
}
