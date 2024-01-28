

#include "ConfigException.hpp"

ConfigException::ConfigException() : message("Parser Error")
{}

ConfigException::ConfigException( const std::string& msg, const std::string& directive, const std::string & value ) {
		message = "\e[1m\e[31mconfig error\e[0m: " 
				+ directive + ": " + value + "\n\t" + msg;
}

ConfigException::~ConfigException( void ) throw () {}

const char*	ConfigException::what( void ) const throw()
{
	return (message.c_str());
}
