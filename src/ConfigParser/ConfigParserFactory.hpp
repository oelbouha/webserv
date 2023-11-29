/*	                     __          _
 *	   __  ___________ _/ /___ ___  (_)
 *	  / / / / ___/ __ `/ / __ `__ \/ /
 *	 / /_/ (__  ) /_/ / / / / / / / / 
 *	 \__, /____/\__,_/_/_/ /_/ /_/_/ 
 *	/____/	User: Youssef Salmi
 *			File: ConfigParserFactory.hpp 
 */

#pragma once
#ifndef CONFIGPARSERFACTORY_HPP
#define CONFIGPARSERFACTORY_HPP

#include <iostream>

#include "IConfigParser.hpp"
#include "ConfigParser.hpp"

namespace factory
{
	IConfigParser*	makeConfigParser( const string& aConfigFilePath );
}
#endif
