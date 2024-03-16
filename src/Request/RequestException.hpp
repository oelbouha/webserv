/*	                     __          _
 *	   __  ___________ _/ /___ ___  (_)
 *	  / / / / ___/ __ `/ / __ `__ \/ /
 *	 / /_/ (__  ) /_/ / / / / / / / /
 *	 \__, /____/\__,_/_/_/ /_/ /_/_/
 *	/____/	User: Youssef Salmi
 *			File: RequestException.hpp
 */

#pragma once
#ifndef REQUESTEXCEPTION_HPP
#define REQUESTEXCEPTION_HPP

#include <cstring>
#include <iostream>
#include <string>


#include "Logger.hpp"

class RequestException : public std::exception {
  std::string mMessage;

public:
  enum Error {
      CONNECTION_CLOSED,
      BAD_REQUEST,
      DEFAULT
  };

public:
  RequestException();
  RequestException(const std::string& aMessage, Error error = DEFAULT);
  RequestException(Error error);

  ~RequestException() throw();
  const char *what() const throw();

  Error error;
};
#endif
