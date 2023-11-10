/*	                     __          _
 *	   __  ___________ _/ /___ ___  (_)
 *	  / / / / ___/ __ `/ / __ `__ \/ /
 *	 / /_/ (__  ) /_/ / / / / / / / / 
 *	 \__, /____/\__,_/_/_/ /_/ /_/_/ 
 *	/____/	User: Youssef Salmi
 *			File: ParserException.hpp 
 */

#pragma once
#ifndef PARSEREXCEPTION_HPP
#define PARSEREXCEPTION_HPP

#include <iostream>
#include <string>
#include <cstring>

class ParserException : public std::exception
{
	std::string	mMsg;

	protected:

	public:
		ParserException();
		ParserException( std::string& aMsg );
		~ParserException() throw ();
		const char*	what( void ) const throw();

};
#endif
