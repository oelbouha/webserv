/*	                     __          _
 *	   __  ___________ _/ /___ ___  (_)
 *	  / / / / ___/ __ `/ / __ `__ \/ /
 *	 / /_/ (__  ) /_/ / / / / / / / / 
 *	 \__, /____/\__,_/_/_/ /_/ /_/_/ 
 *	/____/	User: Youssef Salmi
 *			File: ConfigHelper.hpp 
 */

#pragma once
#ifndef CONFIGHELPER_HPP
#define CONFIGHELPER_HPP

#define LIST_END ""

#include <fstream>
#include <map>
#include <vector>
#include <string>
#include <algorithm>

using std::map;
using std::vector;
using std::string;
using std::fstream;

typedef map<const string, vector<string> >	AllowedValues;

class ConfigHelper
{
	ConfigHelper();

	static const string         sBlockConfigs[];
	static const string			sListConfigs[];
	static const string			sInlineConfigs[];
    static const AllowedValues  sAllowedValues;

	public:
		static AllowedValues	initAllowedValues();
		static bool	isBlockConfig(const string& aConfig);
		static bool	isValueAllowed(const string& aConfig, const string& aValue);
		static bool	doesConfigAcceptAll(const string& aConfig);
		static bool	isListConfig(const string& aConfig);
		static bool	isInlineConfig(const string& aConfig);

};
#endif
