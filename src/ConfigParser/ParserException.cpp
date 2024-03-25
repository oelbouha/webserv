/*	                     __          _
 *	   __  ___________ _/ /___ ___  (_)
 *	  / / / / ___/ __ `/ / __ `__ \/ /
 *	 / /_/ (__  ) /_/ / / / / / / / / 
 *	 \__, /____/\__,_/_/_/ /_/ /_/_/ 
 *	/____/	User: Youssef Salmi
 *			File: ParserException.cpp 
 */

#include "ParserException.hpp"

ParserException::ParserException() : mMessage("Parser Error")
{}

ParserException::ParserException( const std::string& aFileName, const std::string& aError, int aLine ) :
mFileName(aFileName),
mError(aError),
mLine(aLine)
{
	if (mFileName.empty())
		mMessage = std::string("\e[1m\e[31mconfig error\e[0m : ConfigParser error");
	else
		mMessage = "\e[1m\e[31mconfig error\e[0m: "
					+ mFileName
					+ ":" + utils::to_string(mLine) + "\n\t"
					+ mError;
}

ParserException::ParserException( const std::string& aFileName, const std::string& aError ) :
mFileName(aFileName),
mError(aError)
{
	if (mFileName.empty())
		mMessage = std::string("\e[1m\e[31mconfig error\e[0m : ConfigParser error");
	else
		mMessage = "\e[1m\e[31mconfig error\e[0m: "
					+ mFileName
					+ ":\n\t"
					+ mError;
}

ParserException::~ParserException( void ) throw ()
{}

const char*	ParserException::what( void ) const throw()
{
	return (mMessage.c_str());
}

