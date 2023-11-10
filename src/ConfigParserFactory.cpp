/*	                     __          _
 *	   __  ___________ _/ /___ ___  (_)
 *	  / / / / ___/ __ `/ / __ `__ \/ /
 *	 / /_/ (__  ) /_/ / / / / / / / / 
 *	 \__, /____/\__,_/_/_/ /_/ /_/_/ 
 *	/____/	User: Youssef Salmi
 *			File: ConfigParserFactory.cpp 
 */

#include "ConfigParserFactory.hpp"

IConfigParser*	ConfigParserFactory::make( const string& aConfigFilePath )
{
	return (new ConfigParser(aConfigFilePath));
}
