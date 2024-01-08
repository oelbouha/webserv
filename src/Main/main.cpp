
#include <iostream>
#include <fstream>
#include <string>

#include "WebServer.hpp"

#include "src/Interfaces/IConfigParser.hpp"

#include "src/DataTypes/Config.hpp"

#include "src/ConfigParser/ConfigParserFactory.hpp"

#include "Utils.hpp"

int main(int c, char *v[])
{
	std::string	configFilePath("config/example.yml");

	if (c > 1)
		configFilePath = v[1];
	
	try
	{
		const Config&	config = factory::makeConfigParser(configFilePath)->parse();

        Config * cluster = config.getBlockConfig("cluster").front();

        cluster->dump();

        std::vector<Config*>    servers = cluster->getBlockConfig("server");

        servers.front()->dump();

		WebServer		server(&config);

		server.start();
		server.loop();
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
	return (0);
}
