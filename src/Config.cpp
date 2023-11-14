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

vector<Config*>		Config::getBlockConfig(const string& key) const
{
	return (mBlockConfigs.at(key));
}

vector<string>		Config::getListConfig(const string& key) const
{
	return (mListConfig.at(key));
}

const string&		Config::getInlineConfig(const string& key) const
{
	return (mInlineConfig.at(key));
}

void				Config::addBlock(const string& property, Config* value)
{
	mBlockConfigs[property].push_back(value);
}

void				Config::addList(const string& property, const vector<string>& value)
{
	mListConfig[property] = value;
}

void				Config::addInline(const string& property, const string& value)
{
	mInlineConfig[property] = value;
}


void				Config::dump(int indent) const
{
	// std::cout << "==================\n" << "|| " << indent << "\n=================\n" << std::endl;

	std::map<string, string>::const_iterator	iit = mInlineConfig.begin();

	while (iit != mInlineConfig.end())
	{
		std::cout << std::string(indent, '\t');
		std::cout << iit->first << " : " << iit->second << std::endl;
		++iit;
	}

	if (mInlineConfig.size())
		std::cout << std::endl;

	std::map<string, vector<string> >::const_iterator	lit = mListConfig.begin();

	while (lit != mListConfig.end())
	{
		const vector<string>&	v = lit->second;

		std::cout << std::string(indent, '\t') << lit->first << " : \n";

		vector<string>::const_iterator	it = v.begin();
		while (it != v.end())
		{
			std::cout << std::string(indent + 1, '\t') << "-\t" << *it << std::endl;
			++it;
		}
		++lit;
	}

	if (mListConfig.size())
		std::cout << std::endl;

	std::map<string, vector<Config*> >::const_iterator	bit = mBlockConfigs.begin();

	while (bit != mBlockConfigs.end())
	{
		const vector<Config*>&	v = bit->second;
		vector<Config*>::const_iterator	it = v.begin();

		while (it != v.end())
		{
			const Config* const conf = *it;
			std::cout << std::string(indent, '\t') << bit->first << " : \n";
			conf->dump(indent + 1);
			++it;
		}
		++bit;
	}
	
	
}