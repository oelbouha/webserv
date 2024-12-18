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
#include "Utils.hpp"
#include "ConfigException.hpp"

#include "Logger.hpp"

using std::map;
using std::vector;
using std::string;
using std::fstream;

class Config
{
	map<string, vector<Config*> >	mBlockConfig;
	map<string, vector<string> >	mListConfig;
	map<string, string>				mInlineConfig;


	public:
		Config();
		Config( Config& b );
		Config&	operator=( Config& b );
		Config( const fstream& aFileStream );
		~Config();

		vector<Config*>		getBlockConfig(const string& key) const;
		vector<string>		getListConfig(const string& key) const;
		const string&		getInlineConfig(const string& key) const;
	
		vector<Config*>		getBlockConfigIfExist(const string& prop) const;
		vector<string>		getListConfigIfExist(const string& prop) const;
		const string		getInlineConfigIfExist(const string& prop) const;
		const string&		getInlineConfigOr(const string& prop, const string& default_value) const;

		void				addBlock(const string& property, Config* value);
		void				addList(const string& property, const vector<string>& value);
		void				addInline(const string& property, const string& value);

		bool				hasBlock(const string& property) const;
		bool				hasList(const string& property) const ;
		bool				hasInline(const string& property) const;

		void				addInlineIfNotExist(Config& config, const string& prop);
		
		void				addInLineIfExist(Config& config, const string& prop);
		void				addBlockIfExist(Config& config, string prop);
		void				addListIfExist(Config& server, const string& prop);
	
		void	dump(int indent = 0) const;
};
#endif
