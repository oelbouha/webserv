/*	                     __          _
 *	   __  ___________ _/ /___ ___  (_)
 *	  / / / / ___/ __ `/ / __ `__ \/ /
 *	 / /_/ (__  ) /_/ / / / / / / / / 
 *	 \__, /____/\__,_/_/_/ /_/ /_/_/ 
 *	/____/	User: Youssef Salmi
 *			File: YmlConfigParser.cpp 
 */

#include "YmlConfigParser.hpp"


YmlConfigParser::YmlConfigParser()
{}

YmlConfigParser::YmlConfigParser( const YmlConfigParser& c )
{(void)c;}

//TODO: Throw ParserException instead of std::invalid_argument.
YmlConfigParser::YmlConfigParser( const string& aConfigFile ) : mFileName(aConfigFile)
{
	mFileStream.open(aConfigFile.c_str());
	mFileStream.peek();
	if (mFileStream.fail())
	{
		// throw ParserException("File could NOT be opened");
		throw std::invalid_argument("File could NOT be opened");

	}
	else if (mFileStream.eof())
	{
		//throw ParserException("File Empy")
		throw std::invalid_argument("File Empy");
	}
}

YmlConfigParser::~YmlConfigParser()
{
	mFileStream.close();
}


YmlConfigParser&	YmlConfigParser::operator=( const YmlConfigParser& c )
{
	(void)c;
	return (*this);
}


Config*	YmlConfigParser::parse()
{
	mConfig = parseBlock_("global");
	return (mConfig);
}

Config&	YmlConfigParser::getConfig()
{
	return (*mConfig);
}


//	PRIVATE METHODS
Config*			YmlConfigParser::parseBlock_(const string& aKey, int aTabCount)
{
	Config*	config = new Config();
	string	line;

	int		block_tabs_count = 0;
	int		leading_tabs_count = 0;

	string	property;

	while (mFileStream)
	{
		std::getline(mFileStream, line);
		++mLineNumber;

		if (isComment_(line))
			continue ;

		leading_tabs_count = countLeadingTabs_(line);

		if (aTabCount != -1 && !block_tabs_count)
			block_tabs_count = leading_tabs_count;

		if (aTabCount != -1 && leading_tabs_count <= aTabCount)
        {
            mFileStream.seekg( (size_t)mFileStream.tellg() - line.length() - 1 );
			break ;
        }
		else if (leading_tabs_count != block_tabs_count)
			throw ParserException(mFileName, "Misaligned Indentations", mLineNumber);

		property = extractProperty_(line);

		if (!ConfigHelper::isValueAllowed(aKey, property))
			throw ParserException(mFileName,
				aKey + " does not have a `" + property + "` property",
				mLineNumber);

		else if (ConfigHelper::isInlineConfig(property))
		{
			string	value = parseInline_(property, line);
            config->addInline(
				property,
				value
			);
		}
		
		else if (ConfigHelper::isListConfig(property))
		{
			vector<string>	list = parseList_(property, block_tabs_count);
			config->addList(
				property,
				list
			);
		}
		
		else if (ConfigHelper::isBlockConfig(property))
		{
			config->addBlock(
				property,
				parseBlock_(property, block_tabs_count)
			);
		}

		else
			throw ParserException(mFileName, "Unkown property : " + property, mLineNumber);
	}
	return (config);
}


vector<string>	YmlConfigParser::parseList_(const string& aKey, int aTabCount)
{
	vector<string>	list;
	string			value;
	string			line;
	int				tabCount;

	while (true)
	{
		std::getline(mFileStream, line);
		++mLineNumber;

		tabCount = countLeadingTabs_(line);

		if (tabCount > aTabCount + 1)
			throw ParserException(mFileName, "Misaligned Indentations", mLineNumber);
		else if (tabCount <= aTabCount)
        {
            mFileStream.seekg( (size_t)mFileStream.tellg() - line.length() - 1 );
			break ;
        }

		line = line.substr(line.find_first_not_of(" \t"));

		if (line.empty() || isComment_(line))
			continue;

		if( line[0] != '-')
			throw ParserException(mFileName, "Misaligned Indentations", mLineNumber);
		if (line[1] != ' ' && line[1] != '\t')
			throw ParserException(mFileName, "Expected list item (example: - value)", mLineNumber);

		line = line.substr(2);

		int first_non_space_char = line.find_first_not_of(" \t");
		int	last_non_space_char = line.length() - 1;
		while (line[last_non_space_char] == ' ' || line[last_non_space_char] == '\t')
			--last_non_space_char;
		++last_non_space_char;

		value = line.substr(first_non_space_char, last_non_space_char - first_non_space_char);

        if (!ConfigHelper::isValueAllowed(aKey, value))
			    throw ParserException(mFileName,
				    aKey + " can NOT be set to `" + value + "`",
				    mLineNumber);

        
		list.push_back(value);
	}
	return list;
}

string			YmlConfigParser::parseInline_(const string& aKey, const string& line)
{
	std::size_t	first_non_space_char = line.find_first_not_of(" \t", line.find(':') + 1);
	std::size_t last_non_space_char = utils::find_last_not_of(line, " \t");
	
	if (first_non_space_char == std::string::npos)
		throw ParserException(mFileName, "Empty field", mLineNumber);

	else if (!std::isspace(line[first_non_space_char - 1]))
		throw ParserException(mFileName, "Invalid Syntax - Missing space after collon", mLineNumber);

	string	value = line.substr(first_non_space_char, last_non_space_char - first_non_space_char);

    if (!ConfigHelper::isValueAllowed(aKey, value))
			    throw ParserException(mFileName,
				    aKey + " can NOT be set to `" + value + "`",
				    mLineNumber);


	return (value);
}

unsigned int	YmlConfigParser::countLeadingTabs_(const string& line) const
{
	int		count = 0;

	if (line[0] == ' ')// using spaces - 2spaces = 1tab 15000
	{
		for(;;count += 2)
		{
			if (!std::isspace(line[count]))
				return (count / 2);
		
			else if (line[count] == ' ' && line[count + 1] == ' ')
				continue ;

			else if (line[count] == '\t' || line[count + 1] == '\t')
				throw ParserException(mFileName, "Mixing spaces/tabs", mLineNumber);
			throw ParserException(mFileName, "Uneven Indentation", mLineNumber);
		}
	}
	else if (line[0] == '\t')// using tabs
	{
		for(;;++count)
		{
			if (line[count] != ' ' && line[count] != '\t')
				return (count);

			else if (line[count] == '\t')
				continue ;

			throw ParserException(mFileName, "Mixing spaces/tabs", mLineNumber);
		}
	}
	return (count);
}

bool			YmlConfigParser::isComment_(const string& line) const
{
	std::size_t	pos = line.find_first_not_of(" \t");
	
	if (pos == std::string::npos)
		return (true);
	return (line[pos] == '#');
}

string			YmlConfigParser::extractProperty_(const string& line) const
{
	std::size_t	collon_pos = line.find(':');
	if (collon_pos == std::string::npos)
		throw ParserException(mFileName, "Invalid Syntax", mLineNumber);
	
	std::size_t	first_non_space_letter_pos = line.find_first_not_of(" \t", 0);
	std::string property = line.substr(first_non_space_letter_pos, collon_pos - first_non_space_letter_pos);

	std::size_t	last_non_space_char_pos = utils::find_last_not_of(property, " \t");
	return (property.substr(0, last_non_space_char_pos));
}
