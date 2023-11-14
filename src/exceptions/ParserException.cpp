/*	                     __          _
 *	   __  ___________ _/ /___ ___  (_)
 *	  / / / / ___/ __ `/ / __ `__ \/ /
 *	 / /_/ (__  ) /_/ / / / / / / / / 
 *	 \__, /____/\__,_/_/_/ /_/ /_/_/ 
 *	/____/	User: Youssef Salmi
 *			File: ParserException.cpp 
 */

#include "ParserException.hpp"

ParserException::ParserException( void )
{}

ParserException::ParserException( const std::string& aFileName, const std::string& aError, int aLine ) :
mFileName(aFileName),
mError(aError),
mLine(aLine)
{}

ParserException::~ParserException( void ) throw ()
{}

const char*	ParserException::what( void ) const throw()
{
	std::string	message;
	if (mFileName.empty())
		message = "\e[1m\e[31mconfig error\e[0m : ConfigParser error";
	else
		message = "\e[1m\e[31mconfig error\e[0m: "
					+ mFileName
					+ ":" + std::to_string(mLine) + "\n\t"
					+ mError;

	return (message.c_str());
}

