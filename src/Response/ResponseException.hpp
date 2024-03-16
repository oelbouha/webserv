/*	                     __          _
 *	   __  ___________ _/ /___ ___  (_)
 *	  / / / / ___/ __ `/ / __ `__ \/ /
 *	 / /_/ (__  ) /_/ / / / / / / / / 
 *	 \__, /____/\__,_/_/_/ /_/ /_/_/ 
 *	/____/	User: Youssef Salmi
 *			File: ResponsException.hpp 
 */

#pragma once
#ifndef RESPONSEEXCEPTION_HPP
#define RESPONSEEXCEPTION_HPP

#include <iostream>
#include <string>
#include <cstring>

class ResponseException : public std::exception
{
	std::string	mMessage;

public:
  enum Error {
      FILE_NOT_FOUND,
      PERMISSION_DENIED
  };
  Error error;
  
public:
	ResponseException();
	ResponseException( const std::string& msg, Error error = FILE_NOT_FOUND );
	ResponseException( Error error );
	~ResponseException() throw();
	const char*	what() const throw();
};

#endif
