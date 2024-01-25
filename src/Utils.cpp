/*	                     __          _
 *	   __  ___________ _/ /___ ___  (_)
 *	  / / / / ___/ __ `/ / __ `__ \/ /
 *	 / /_/ (__  ) /_/ / / / / / / / / 
 *	 \__, /____/\__,_/_/_/ /_/ /_/_/ 
 *	/____/	User: Youssef Salmi
 *			File: Utils.cpp 
 */

#include "Utils.hpp"
#include <sstream>

namespace utils
{
	std::string&	replace_all(std::string& str, const std::string& old_word, const std::string& new_word)
	{
		std::string::size_type n = 0;
		while ( ( n = str.find( old_word, n ) ) != std::string::npos )
		{
			str.replace( n, old_word.length(), new_word );
			n += old_word.length();
		}
		return (str);
	}

	std::size_t	find_last_not_of(const std::string& s, const std::string& set, std::size_t start)
	{
		std::size_t pos = s.length() - 1;

		while (pos != std::string::npos && set.find(s[pos]) != std::string::npos)
			--pos;
		++pos;
		if (pos >= start)
			return (pos);
		return (std::string::npos);
	}

	unsigned int	ip(unsigned char a, unsigned char b, unsigned char c, unsigned char d)
	{
		return (a << 24 | b << 16 | c << 8 | d);
	}

	std::string		ip(unsigned int aIP)
	{
		std::string		ret;
		unsigned char	byte = aIP >> 0;

		ret += std::to_string(byte) + ".";
		byte = aIP >> 24 & 255;
		ret += std::to_string(byte) + ".";
		byte = aIP >> 16 & 255;
		ret += std::to_string(byte) + ".";
		byte = aIP >> 8 & 255;
		ret += std::to_string(byte);
		return (ret);
	}

	std::string UintToIp(unsigned int ip_int) {
		std::stringstream ip_stream;

		unsigned char octet1 = (ip_int >> 24) & 0xFF;
		unsigned char octet2 = (ip_int >> 16) & 0xFF;
		unsigned char octet3 = (ip_int >> 8) & 0xFF;
		unsigned char octet4 = ip_int & 0xFF;

		ip_stream << static_cast<int>(octet1) << "." << static_cast<int>(octet2) << "."
				<< static_cast<int>(octet3) << "." << static_cast<int>(octet4);

		return ip_stream.str();
	}
	bool	isNotSpace(char c)
	{
		return (std::isspace(c) == false);
	}

	std::string&	trimSpaces(std::string& s)
	{
		s.erase(s.begin(), 
			std::find_if( s.begin(), s.end(), isNotSpace ));
		s.erase( std::find_if( s.rbegin(), s.rend(), isNotSpace ).base(), 
			s.end());
		return (s);
	}

	int	get_exit_status(pid_t pid)
	{
		int				ret;
		unsigned char	*status;
		int				stts;

		stts = 0;
		waitpid(pid, &stts, 0);
		status = (unsigned char *) &stts;
		if (status[0])
			return (status[0] + 128);
		ret = (unsigned char) status[1];
		return (ret);
	}
	bool	IsDirectory(string uri)
	{
		struct stat info;

		if (stat(uri.c_str(), &info) != 0)
			return false;
		// Check if the path corresponds to a directory
		return S_ISDIR(info.st_mode);
	}

	string	getExtension(string line)
	{
		string extension;
		int pos = line.rfind('.');
		if (pos > 0)
			extension = line.substr(pos + 1, line.length());
		return (extension);	
	}

	bool isValidNumber(std::string line) {
		for(size_t i = 0; i < line.length(); ++i) {
			if (std::isdigit(line[i]) == false)
				return false;
		}
		return true;
	}

	bool isValidIp_address(std::string& ip_address) {
		std::istringstream ss(ip_address);
		std::string component;
		unsigned int count = 0;

		while (std::getline(ss, component, '.')) {
			if (isValidNumber(component) == false) {
				return 0;
			}
			++count;
		}
		return (count == 4);
	}

	unsigned int ipToUint(const std::string& ip_address) {
		std::vector<int> components;
		std::istringstream ss(ip_address);
		std::string component;

		while (std::getline(ss, component, '.')) {
			if (isValidNumber(component) == false) {
				std::cerr << "Invalid IPv4 address format." << std::endl;
				return 0;
			}
			components.push_back(std::stod(component, NULL));
		}

		if (components.size() != 4) {
			std::cerr << "Invalid IPv4 address format." << std::endl;
			return 0;
		}

		unsigned int result = (components[0] << 24) | (components[1] << 16) | (components[2] << 8) | components[3];
		return result;
	}

	int	stringToInt(std::string line) {
		if (isValidNumber(line) == false)
			return (0);
		return std::stod(line, NULL);
	}

	std::vector<string> SplitString(std::string line, char delimeter) {
		std::vector<string> vec;
		std::stringstream ss(line);
		std::string component;

		while (std::getline(ss, component, delimeter)) {
			std::vector<string>::iterator find = std::find(vec.begin(), vec.end(), component);
			if (!component.empty() && find == vec.end())
				vec.push_back(component);
		}

		return vec;
	}
}
