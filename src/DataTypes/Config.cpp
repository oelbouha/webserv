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
{
	map<string, vector<Config*> >::iterator	mapIt = mBlockConfig.begin();

	while (mapIt != mBlockConfig.end())
	{
		// mapIt->second is a vector of Config*
		vector<Config*>&	vec = mapIt->second;
		vector<Config*>::iterator vectorIt = vec.begin();

		while (vectorIt != vec.end())
		{
			// *vectorIt is a Config*
			delete *vectorIt;
			++vectorIt;
		}
		++mapIt;
	}
}

Config&	Config::operator=( Config& c )
{
	(void)c;
	return (*this);
}

vector<Config*>		Config::getBlockConfig(const string& key) const
{
	return (mBlockConfig.at(key));
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
	mBlockConfig[property].push_back(value);
}

void				Config::addList(const string& property, const vector<string>& value)
{
	mListConfig[property] = value;
}

void				Config::addInline(const string& property, const string& value)
{
	mInlineConfig[property] = value;
}

bool		Config::hasBlock(const string& property) const
{
	try {
		mBlockConfig.at(property);
		return (true);
	}
	catch(...){
		return (false);
	}
}

bool		Config::hasList(const string& property) const 
{
	try {
		mListConfig.at(property);
		return (true);
	}
	catch(...){
		return (false);
	}
}

bool		Config::hasInline(const string& property) const
{
	try {
		mInlineConfig.at(property);
		return (true);
	}
	catch(...){
		return (false);
	}
}


void	Config::addInLineIfExist(Config& config, const string& prop)
{
	if (prop.empty())
		return ;

	if (config.hasInline(prop) == true)
		addInline(prop, config.getInlineConfig(prop));
}


void	Config::addInlineIfNotExist(Config& config, const string& prop)
{
	if (prop.empty())
		return ;

	if ( ! hasInline(prop) )
		addInLineIfExist(config, prop);
}

void	Config::addBlockIfExist(Config& server, string prop)
{
	if (prop.empty())
		return ;
	if (server.hasBlock(prop) == true)
	{
		std::vector<Config*> error_page = server.getBlockConfig(prop);
		std::vector<Config*>::iterator it = error_page.begin();
		while (it != error_page.end()){
				addBlock(prop, *it);
			++it;
		}
	}
}

void	Config::addListIfExist(Config& server, const string& prop)
{
	if (prop.empty())
		return ;
	if (server.hasList(prop) == true)
		addList(prop, server.getListConfig(prop));
}


vector<Config*>	Config::getBlockConfigIfExist(const string& property) const 
{
	vector<Config*> ret;
	if (hasBlock(property) == true){
		ret = getBlockConfig(property);
	}
	return ret;
}

vector<string>	Config::getListConfigIfExist(const string& property) const 
{
	vector<string> ret;
	if (hasList(property) == true)
		ret = getListConfig(property);
	return ret;
}

const string 	Config::getInlineConfigIfExist(const string& property) const 
{
	if (hasInline(property) == true){
		return (getInlineConfig(property));
	}
	return "";
}

const string& 	Config::getInlineConfigOr(const string& property, const string& default_value) const 
{
	if (hasInline(property) == true){
		return (getInlineConfig(property));
	}
	return default_value;
}

// void	Config::IsValidDirective(const std::string& property) {
// 	std::string value;
// 	if (property == "host")
// 	{
// 		value = getInlineConfigIfExist(property);
// 		if (utils::isValidIp_address(value) == false)
// 			throw ConfigException("Webserver: Invalid IPv4 address format.", property, value);
// 		return ;
// 	}
// 	value = getInlineConfigIfExist(property);
// 	std::vector<string> ports = utils::SplitString(value, ' ');
// 	for(size_t i = 0; i < ports.size(); ++i)
// 	{
// 		if (utils::isValidNumber(ports[i]) == false)
// 			throw ConfigException("Webserver : Not a Valid Number", property, ports[i]);
// 	}
// } 


/****************************************************************************************************/
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

	std::map<string, vector<Config*> >::const_iterator	bit = mBlockConfig.begin();

	while (bit != mBlockConfig.end())
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