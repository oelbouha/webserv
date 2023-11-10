/*	                     __          _
 *	   __  ___________ _/ /___ ___  (_)
 *	  / / / / ___/ __ `/ / __ `__ \/ /
 *	 / /_/ (__  ) /_/ / / / / / / / / 
 *	 \__, /____/\__,_/_/_/ /_/ /_/_/ 
 *	/____/	User: Youssef Salmi
 *			File: ConfigParser.cpp 
 */

#include "ConfigParser.hpp"


ConfigParser::ConfigParser()
{}

ConfigParser::ConfigParser( ConfigParser& c )
{}

//
//TODO: Throw ParserException instead of std::invalid_argument.
//

ConfigParser::ConfigParser( const string& aConfigfilePath )
{
	mFileStream.open(aConfigfilePath.c_str());
	mFileStream.peek();
	if (mFileStream.fail())
	{
		//throw ParserException("File could NOT be opened");
		throw std::invalid_argument("File could NOT be opened");

	}
	else if (mFileStream.eof())
	{
		//throw ParserException("File Empy")
		throw std::invalid_argument("File Empy");
	}
}

ConfigParser::~ConfigParser()
{
	mFileStream.close();
}

ConfigParser&	ConfigParser::operator=( ConfigParser& c )
{
	return (*this);
}

Config*			ConfigParser::parseBlock_(const string& key)
{
	Config*	config = new Config();
	string	line;
	int		block_tabs_count;
	int		leading_tabs_count;

	string	property;


	while (true)
	{
		std::getline(mFileStream, line);

		if (line.empty() || isComment_(line))
			continue ;

		leading_tabs_count = countLeadingTabs_(line);
		if (leading_tabs_count < block_tabs_count)
			break ;

		property = extractProperty_(line);

		if (!ConfigHelper::isPropertyOfBlock(key, property))
		{
			//throw ParserException(key + "does not have a `" + property + "` property");
		}

		else if (ConfigHelper::isInlineConfig(property))
		{
			string	value = parseInline_();
			config->addInline(
				property,
				value
			);
		}
		
		else if (ConfigHelper::isListConfig(property))
		{
			vector<string>	list = parseList_();
			config->addList(
				property,
				list
			);
		}
		
		else if (ConfigHelper::isBlockConfig(property))
		{
			config->addBlock(
				property,
				parseBlock_(property)
			);
		}
	}
	return (config);
}

const Config&	ConfigParser::parse()
{
	string	line;
	while (mFileStream){
		std::getline(mFileStream, line);
		if (line.empty())
			continue ;
		
	}
}

const Config&	ConfigParser::getConfig()
{
	return (mConfig);
}
