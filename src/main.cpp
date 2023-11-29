
#include <iostream>
#include <fstream>
#include <string>

#include "WebServer.hpp"

#include "src/Interfaces/IConfigParser.hpp"

#include "src/DataTypes/Config.hpp"

#include "src/ConfigParser/ConfigParserFactory.hpp"

#include "Utils.hpp"

int main(int __unused c, char __unused **v)
{
	std::string	configFilePath("defaultPath");

	if (c > 1)
		configFilePath = v[1];
	
	try
	{
		const Config&	config = factory::makeConfigParser(configFilePath)->parse();
		WebServer		server(&config);

		server.start();
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
	return (0);
}