
#include <iostream>
#include <fstream>
#include <string>

#include "WebServer.hpp"
#include "IConfigParser.hpp"
#include "ConfigParserFactory.hpp"
#include "Config.hpp"

#include "ConfigHelper.hpp"
#include "Utils.hpp"

#include "Socket.hpp"

using	std::string;
using	std::cout;
using	std::endl;

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