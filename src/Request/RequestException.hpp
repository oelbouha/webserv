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

#include <iostream>
#include <string>
#include <cstring>

class RequestException : public std::exception
{
	std::string	mMessage;

public:
	RequestException();
	RequestException( const std::string& aMessage );
	~RequestException() throw();
	const char*	what() const throw();

};
#endif
