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
#include <sstream>

#include "IConfigParser.hpp"
#include "ConfigHelper.hpp"

using	std::ifstream;
using	std::stringstream;

class ConfigParser : public IConfigParser
{
	Config		mConfig;
	ifstream	mFileStream;


	ConfigParser();
	ConfigParser( ConfigParser& c );
	ConfigParser&	operator=( ConfigParser& c );

	// Class Utilities
	unsigned int	countLeadingTabs_(const string& line);
	bool			isComment_(const string& line);
	string			extractProperty_(const string& line);

	// Class Parsing Units
	Config*			parseBlock_(const string& key);
	vector<string>	parseList_();
	string			parseInline_();

	public:
		ConfigParser( const string& aConfigFilePath );
		~ConfigParser();


		const Config&	parse();
		const Config&	getConfig();

};


#endif
