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
      BAD_REQUEST = 400,
      HEADER_TOO_LARGE = 431,
      CLIENT_BODY_TOO_LARGE = 413
  };

public:
  RequestException();
  RequestException(const std::string& aMessage, Error error = BAD_REQUEST);
  RequestException(Error error);

  ~RequestException() throw();
  const char *what() const throw();

  Error error;
};
#endif
