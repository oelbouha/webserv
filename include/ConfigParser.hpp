/*	                     __          _
 *	   __  ___________ _/ /___ ___  (_)
 *	  / / / / ___/ __ `/ / __ `__ \/ /
 *	 / /_/ (__  ) /_/ / / / / / / / / 
 *	 \__, /____/\__,_/_/_/ /_/ /_/_/ 
 *	/____/	User: Youssef Salmi
 *			File: ConfigParser.hpp 
 */

#pragma once
#ifndef CONFIGPARSER_HPP
#define CONFIGPARSER_HPP

#include <iostream>
#include <fstream>
#include <string>
#include <cstring>

#include "IConfigParser.hpp"

using	std::ifstream;

class ConfigParser : public IConfigParser
{
	Config		mConfig;
	ifstream	mFileStream;
	bool		mIsParsed;

	ConfigParser();
	ConfigParser( ConfigParser& c );
	ConfigParser&	operator=( ConfigParser& c );

	void	parse();

	public:
		ConfigParser( const string& aConfigFilePath );
		~ConfigParser();


		const Config&	getConfig();

};
#endif
