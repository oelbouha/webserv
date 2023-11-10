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

const Config&	ConfigParser::parse()
{
	
}

const Config&	ConfigParser::getConfig()
{
	return (mConfig);
}