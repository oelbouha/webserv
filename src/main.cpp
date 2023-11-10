
#include <iostream>
#include <fstream>
#include <string>

// #include "IConfigParser.hpp"
// #include "ConfigParserFactory.hpp"
// #include "Config.hpp"

#include "ConfigHelper.hpp"

using	std::string;
using	std::cout;
using	std::endl;

int main(int __unused c, char __unused **v)
{
	// IConfigParser*	parser = ConfigParserFactory::make("config/example.yml");

	bool check = ConfigHelper::isInlineConfig("http");
	if (check)
		std::cout << "true" << std::endl;

    return (0);
}