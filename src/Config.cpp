/*	                     __          _
 *	   __  ___________ _/ /___ ___  (_)
 *	  / / / / ___/ __ `/ / __ `__ \/ /
 *	 / /_/ (__  ) /_/ / / / / / / / / 
 *	 \__, /____/\__,_/_/_/ /_/ /_/_/ 
 *	/____/	User: Youssef Salmi
 *			File: Config.cpp 
 */

#include "Config.hpp"

Config::Config( void )
{}

Config::Config( Config& c )
{}

Config::Config( const fstream& aFileStream )
{
	while (aFileStream)
	{
		
	}
}

Config::~Config()
{}

Config&	Config::operator=( Config& c )
{
	return (*this);
}
