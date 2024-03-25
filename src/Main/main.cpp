#include <iostream>
#include <fstream>
#include <string>

#include <signal.h>

#include "src/Main/WebServer.hpp"

#include "src/Interfaces/IConfigParser.hpp"

#include "src/DataTypes/Config.hpp"

#include "src/ConfigParser/ConfigParserFactory.hpp"

#include "Utils.hpp"

int main(int c, char *v[])
{
	std::string	configFilePath("config/ysalmi.yml");
	if (c > 1) configFilePath = v[1];

	try
	{
		IConfigParser*	parser = factory::makeConfigParser(configFilePath);
		Config*			config = parser->parse();
		WebServer		server(config);

		signal(SIGPIPE, SIG_IGN);
		server.start();
		server.loop();

		return (0);
	}
	catch(const std::exception& e)
	{ Logger::error (e.what()).flush(); return 1; }
}
