/*	                     __          _
 *	   __  ___________ _/ /___ ___  (_)
 *	  / / / / ___/ __ `/ / __ `__ \/ /
 *	 / /_/ (__  ) /_/ / / / / / / / / 
 *	 \__, /____/\__,_/_/_/ /_/ /_/_/ 
 *	/____/	User: Youssef Salmi
 *			File: Config.cpp 
 */

#include "Config.hpp"

Config::Config()
{}

Config::Config( Config& c )
{(void)c;}

Config::Config( const fstream& aFileStream )
{
	(void)aFileStream;
}

Config::~Config()
{}

Config&	Config::operator=( Config& c )
{
	(void)c;
	return (*this);
}

vector<Config*>		Config::getBlockConfigs(const string& key)
{
	return (mBlockConfigs.at(key));
}

vector<string>		Config::getListConfigs(const string& key)
{
	return (mListConfig.at(key));
}

const string&		Config::getInlineConfig(const string& key)
{
	return (mInlineConfig.at(key));
}

void				Config::addBlock(const string& property, Config* value)
{
	mBlockConfigs[property].push_back(value);
}

void				Config::addList(const string& property, vector<string>& value)
{
	mListConfig[property] = value;
}

void				Config::addInline(const string& property, string& value)
{
	mInlineConfig[property] = value;
}
