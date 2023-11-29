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

	"inline_property",
	"prop1",
	LIST_END
};

const string	ConfigHelper::sListConfigs[] = {
	"allowed_methods",

	"list_property",
	"prop2",

	LIST_END
};

BlockConfigsType	ConfigHelper::initBlockConfigs(){
	const string				blockConfigsRaw[] = {
		"global:",
			"http",
			"error_log_file",
			"error_log_level",

			"inline_property",
			"list_property",
			"block_property",
			"test_property",
		
		"block_property:",
			"prop1",
			"prop2",

		"http:",
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

		LIST_END
	};
	map< const string, vector<string> >	blockConfigs;
	string	key;

	for (int i = 0; ! blockConfigsRaw[i].empty(); ++i)
	{
		string	entry = blockConfigsRaw[i];
		if (entry[ entry.length() - 1 ] == ':')
		{
			key = entry.substr(0, entry.length() - 1);
			blockConfigs[key] = vector<string>();
		}
		else
			blockConfigs[key].push_back(entry);
	}
	return (blockConfigs);
}

const BlockConfigsType ConfigHelper::sBlockConfigs = ConfigHelper::initBlockConfigs();

ConfigHelper::ConfigHelper()
{}

bool	ConfigHelper::isBlockConfig(const string& aConfig)
{
	if (aConfig != "global" && ConfigHelper::sBlockConfigs.find(aConfig) != ConfigHelper::sBlockConfigs.end())
		return true;
	return false;
}

bool	ConfigHelper::isPropertyOfBlock(const string& aConfig, const string& aProperty)
{
	vector<string>	properties = ConfigHelper::sBlockConfigs.at(aConfig);

	if (std::find(properties.begin(), properties.end(), aProperty) != properties.end())
		return true;
	return false;
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