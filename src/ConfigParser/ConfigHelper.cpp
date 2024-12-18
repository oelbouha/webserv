/*	                     __          _
 *	   __  ___________ _/ /___ ___  (_)
 *	  / / / / ___/ __ `/ / __ `__ \/ /
 *	 / /_/ (__  ) /_/ / / / / / / / / 
 *	 \__, /____/\__,_/_/_/ /_/ /_/_/ 
 *	/____/	User: Youssef Salmi
 *			File: ConfigHelper.cpp 
 */

#include "ConfigHelper.hpp"

const string	ConfigHelper::sInlineConfigs[] = {
	"root",
	"host",
	"index",
	"keep_alive",
	"keep_alive_timeout",
	"uri",
	"error_log_file",
	"error_log_level",
	"server_props",
	"upload",
	"extension",
	"handler",
	"file",
	"default",
	"autoindex",
	"code",
	"location",
	"default_mime",
	"max_body_size",
	LIST_END
};

const string	ConfigHelper::sListConfigs[] = {
	"ports",
	"names",
	"allowed_methods",
	"codes",
	"cgi",
	LIST_END
};

const string    ConfigHelper::sBlockConfigs[] = {
    "cluster",
	"redirect",
	"server",
	"route",
	"error_page",
	"mime_types",

    LIST_END
};


AllowedValues	ConfigHelper::initAllowedValues(){
	const string				allowedValuesRaw[] = {
		"global:",
			"cluster",
			"error_log_file",
			"error_log_level",
		
		"cluster:",
			"default_mime",
			"mime_types",
			"keep_alive",
			"keep_alive_timeout",
			"server_props",
			"error_page",
			"server",
			"max_body_size",

		"mime_types:",
			"*",

		"server:",
			"ports",
			"names",
			"root",
			"index",
			"route",
			"host",
			"upload",
			"cgi",
			"Default",
			"index",
			"error_page",
			"default",
			"max_body_size",

		"default:",
			"yes",
			"no",

		"route:",
			"uri",
			"root",
			"upload",
			"allowed_methods",
			"index",
			"autoindex",
			"error_page",
			"redirect",
			"cgi",

		"redirect:"
			"code",
			"location",

        "server_props:",
            "yes", "no",

        "allowed_methods:",
            "GET", "POST", "DELETE",


        "server_props:",
            "yes", "no",


        "allowed_methods:",
            "GET", "POST", "DELETE",


		LIST_END
	};
	map< const string, vector<string> >	allowedValues;
	string	key;

	for (int i = 0; ! allowedValuesRaw[i].empty(); ++i)
	{
		string	entry = allowedValuesRaw[i];
		if (entry[ entry.length() - 1 ] == ':')
		{
			key = entry.substr(0, entry.length() - 1);
			allowedValues[key] = vector<string>();
		}
		else
			allowedValues[key].push_back(entry);
	}
	return (allowedValues);
}

const AllowedValues ConfigHelper::sAllowedValues = ConfigHelper::initAllowedValues();

ConfigHelper::ConfigHelper()
{}

bool	ConfigHelper::isBlockConfig(const string& aConfig)
{
	for (int i = 0; ! ConfigHelper::sBlockConfigs[i].empty() ; ++i)
		if (ConfigHelper::sBlockConfigs[i] == aConfig)
			return true;
	return false;
}

bool	ConfigHelper::isValueAllowed(const string& aConfig, const string& aValue)
{
    try{
	    vector<string>	properties = ConfigHelper::sAllowedValues.at(aConfig);

	    if (std::find(properties.begin(), properties.end(), aValue) != properties.end())
		    return true;
        return false;
    }
    catch (...)
    {
        return true;
    }
}

bool	ConfigHelper::doesConfigAcceptAll(const string& aConfig)
{
    try {
	    vector<string>	properties = ConfigHelper::sAllowedValues.at(aConfig); 
		std::string		aValue = "*";

	    if (std::find(properties.begin(), properties.end(), aValue) != properties.end())
		    return true;
        return false;
    }
    catch (...)
    {
        return false;
    }
}

bool	ConfigHelper::isListConfig(const string& aConfig)
{
	for (int i = 0; ! ConfigHelper::sListConfigs[i].empty() ; ++i)
		if (ConfigHelper::sListConfigs[i] == aConfig)
			return true;
	return false;
}

bool	ConfigHelper::isInlineConfig(const string& aConfig)
{
	for (int i = 0; ! ConfigHelper::sInlineConfigs[i].empty() ; ++i)
		if (ConfigHelper::sInlineConfigs[i] == aConfig)
			return true;
	return false;
}
