
#include <iostream>
#include <fstream>
#include <string>

#include "IConfigParser.hpp"
#include "ConfigParserFactory.hpp"
#include "Config.hpp"

#include "ConfigHelper.hpp"
#include "Utils.hpp"

using	std::string;
using	std::cout;
using	std::endl;

int main(int __unused c, char __unused **v)
{
	try {
		IConfigParser*	parser = factory::makeConfigParser("config/example.yml");
		const Config&	conf = parser->parse();

		conf.dump();
	} catch (const ParserException& e)
	{
		cout << e.what() << endl;
	}
	catch (const std::exception& e)
	{
		cout << "not a ParserException : " << e.what() << endl;
	}

    return (0);
}