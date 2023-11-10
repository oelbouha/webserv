
#include <iostream>
#include <fstream>
#include <string>

// #include "IConfigParser.hpp"
// #include "ConfigParserFactory.hpp"
// #include "Config.hpp"

int main(int __unused c, char **v)
{
	IConfigParser*	parser = ConfigParserFactory::make("config/example.yml");
	// IConfigParser*	parser = ConfigParser("config/example.yml");

	// std::ifstream	infile;

	// infile.exceptions( std::ifstream::failbit | std::ifstream::badbit );
	// try{
	// 	infile.open(v[1]);
	// 	infile.peek();
	// 	if (infile.fail() || infile.eof())
	// 		std::cerr << "infile error" << std::endl;
	// } catch(std::exception& e)
	// {
	// 	std::cerr << e.what() << std::endl;
	// }
    return (0);
}