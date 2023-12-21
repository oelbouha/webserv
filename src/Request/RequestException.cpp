/*	                     __          _
 *	   __  ___________ _/ /___ ___  (_)
 *	  / / / / ___/ __ `/ / __ `__ \/ /
 *	 / /_/ (__  ) /_/ / / / / / / / /
 *	 \__, /____/\__,_/_/_/ /_/ /_/_/
 *	/____/	User: Youssef Salmi
 *			File: RequestException.cpp
 */

#include "RequestException.hpp"

RequestException::RequestException() : mMessage(""), error(DEFAULT) {}

RequestException::RequestException(const std::string &aMessage, RequestException::Error error)
    : mMessage(aMessage), error(error) {}

RequestException::RequestException(const std::string &aMessage)
    : mMessage(aMessage), error(DEFAULT) {}

RequestException::RequestException(RequestException::Error error)
    : mMessage(""), error(error) {}

RequestException::~RequestException() throw() {}

const char *RequestException::what() const throw() {
  return (mMessage.c_str());
}
