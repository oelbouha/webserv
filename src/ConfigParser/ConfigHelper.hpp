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

#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <string>

using std::map;
using std::vector;
using std::string;
using std::fstream;

typedef map<const string, vector<string> >	BlockConfigsType;

class ConfigHelper
{
	ConfigHelper();

	static const BlockConfigsType	sBlockConfigs;
	static const string				sListConfigs[];
	static const string				sInlineConfigs[];

	public:
		static BlockConfigsType	initBlockConfigs();
		static bool	isBlockConfig(const string& aConfig);
		static bool	isPropertyOfBlock(const string& aConfig, const string& aProperty);
		static bool	isListConfig(const string& aConfig);
		static bool	isInlineConfig(const string& aConfig);

};
#endif
