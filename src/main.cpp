
#include <iostream>
#include <fstream>
#include <string>

// #include "IConfigParser.hpp"
// #include "ConfigParserFactory.hpp"
// #include "Config.hpp"

using	std::string;
using	std::cout;
using	std::endl;

int main(int __unused c, char **v)
{
	// IConfigParser*	parser = ConfigParserFactory::make("config/example.yml");
	const string	blockConfigs[] = {
		"http",
		"server"
	};

	cout << blockConfigs[0] << endl << blockConfigs[1] << endl;

    return (0);
}