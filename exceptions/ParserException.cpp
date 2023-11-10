/*	                     __          _
 *	   __  ___________ _/ /___ ___  (_)
 *	  / / / / ___/ __ `/ / __ `__ \/ /
 *	 / /_/ (__  ) /_/ / / / / / / / / 
 *	 \__, /____/\__,_/_/_/ /_/ /_/_/ 
 *	/____/	User: Youssef Salmi
 *			File: ParserException.cpp 
 */

#include "ParserException.hpp"

ParserException::ParserException( void ) : m_msg("")
{}

ParserException::ParserException( std::string& msg ) : m_msg(msg)
{}

ParserException::~ParserException( void ) throw ()
{}

const char*	ParserException::what( void ) const throw()
{
	return (msg.c_str());
}

