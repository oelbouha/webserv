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
	"port",
	"name",
	"root",
	"index",
	"keep_alive",
	"uri",
	"error_log_file",
	"error_log_level",
	"server_props",
	LIST_END
};

const string	ConfigHelper::sListConfigs[] = {
	"allowed_methods",
	LIST_END
};

const string    ConfigHelper::sBlockConfigs[] = {
    "cluster",
  
//bool    ConfigHelper::(const std::string& aConfig, const std::string& aProperty);
"server",
    "route",
    LIST_END
};

AllowedValues	ConfigHelper::initAllowedValues(){
	const string				allowedValuesRaw[] = {
		"global:",
			"cluster",
			"error_log_file",
			"error_log_level",
		
		"cluster:",
			"mime_types",
			"keep_alive",
			"server_props",
			"error_pages",
			"server",
		
		"mime_types:",
			"*",
		
		"server:",
			"port",
			"name",
			"root",
			"index",
			"route",

		"route:",
			"uri",
			"allowed_methods",

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
	    vector<string>	properties = ConfigHelper::sAllowedValues.at(aConfig); // m['server']

	    if (std::find(properties.begin(), properties.end(), aValue) != properties.end())
		    return true;
        return false;
    }
    catch (...)
    {
        return true;
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
