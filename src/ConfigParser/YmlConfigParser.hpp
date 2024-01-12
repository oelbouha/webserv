/*	                     __          _
 *	   __  ___________ _/ /___ ___  (_)
 *	  / / / / ___/ __ `/ / __ `__ \/ /
 *	 / /_/ (__  ) /_/ / / / / / / / / 
 *	 \__, /____/\__,_/_/_/ /_/ /_/_/ 
 *	/____/	User: Youssef Salmi
 *			File: YmlConfigParser.hpp 
 */

#pragma once
#ifndef YMLCONFIGPARSER_HPP
#define YMLCONFIGPARSER_HPP

#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <sstream>

#include "IConfigParser.hpp"
#include "ConfigHelper.hpp"
#include "ParserException.hpp"

#include "Utils.hpp"

using	std::ifstream;
using	std::stringstream;

class YmlConfigParser : public IConfigParser
{
	Config*		mConfig;
	string		mFileName;
	ifstream	mFileStream;
	size_t		mLineNumber;


	YmlConfigParser();
	YmlConfigParser( const YmlConfigParser& c );
	YmlConfigParser&	operator=( const YmlConfigParser& c );

	// Class Utilities
	unsigned int	countLeadingTabs_(const string& line) const;
	bool			isComment_(const string& line) const;
	string			extractProperty_(const string& line) const;

	// Class Parsing Units
	Config*			parseBlock_(const string& aKey, int aTabCount = -1);
	vector<string>	parseList_(const string& aKey, int aTabCount);
	string			parseInline_(const string& aKey, const string& aLine);

	public:
		YmlConfigParser( const string& aConfigFilePath );
		~YmlConfigParser();


		Config*	parse();
		Config&	getConfig();

};


#endif
