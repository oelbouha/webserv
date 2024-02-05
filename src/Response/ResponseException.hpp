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
	ResponseException();
	ResponseException( const std::string& aMessage );
	~ResponseException() throw();
	const char*	what() const throw();

};

#endif
