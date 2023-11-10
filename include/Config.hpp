/*	                     __          _
 *	   __  ___________ _/ /___ ___  (_)
 *	  / / / / ___/ __ `/ / __ `__ \/ /
 *	 / /_/ (__  ) /_/ / / / / / / / / 
 *	 \__, /____/\__,_/_/_/ /_/ /_/_/ 
 *	/____/	User: Youssef Salmi
 *			File: Config.hpp 
 */

#pragma once
#ifndef BLOCKCONFIG_HPP
#define BLOCKCONFIG_HPP

#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <string>

using std::map;
using std::vector;
using std::string;
using std::fstream;

class Config
{
	map<string, vector<Config*> >	mBlockConfigs;
	map<string, vector<string> >	mListConfig;
	map<string, string>				mInlineConfig;


	public:
		Config();
		Config( Config& b );
		Config&	operator=( Config& b );
		Config( const fstream& aFileStream );
		~Config();

		vector<Config*>		getBlockConfigs(const string& key);
		vector<string>		getListConfigs(const string& key);
		const string&		getInlineConfig(const string& key);

		void				addBlock(const string& property, Config* value);
		void				addList(const string& property, vector<string>& value);
		void				addInline(const string& property, string& value);
};
#endif
