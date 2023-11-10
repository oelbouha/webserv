/*	                     __          _
 *	   __  ___________ _/ /___ ___  (_)
 *	  / / / / ___/ __ `/ / __ `__ \/ /
 *	 / /_/ (__  ) /_/ / / / / / / / / 
 *	 \__, /____/\__,_/_/_/ /_/ /_/_/ 
 *	/____/	User: Youssef Salmi
 *			File: ConfigHelper.cpp 
 */

#include "ConfigHelper.hpp"

const string	ConfigHelper::sListConfigs[] = {
	"allowed_methods",
	LIST_END
};

const string	ConfigHelper::sInlineConfigs[] = {
	"port",
	"keep_alive",
	LIST_END
};

ConfigHelper::ConfigHelper()
{}

map< string, vector<string> >	ConfigHelper::initBlockConfigs()
{
	map< string, vector<string> >	blockConfigs;

	const string				blockConfigsRaw[] = {
		"global:",
			"http",
			"error_log_file",
			"error_log_level",

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

		LIST_END
	};

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
