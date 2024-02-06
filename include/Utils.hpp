/*	                     __          _
 *	   __  ___________ _/ /___ ___  (_)
 *	  / / / / ___/ __ `/ / __ `__ \/ /
 *	 / /_/ (__  ) /_/ / / / / / / / / 
 *	 \__, /____/\__,_/_/_/ /_/ /_/_/ 
 *	/____/	User: Youssef Salmi
 *			File: Utils.hpp 
 */

#pragma once
#ifndef UTILS_HPP
#define UTILS_HPP

#include <ostream>
#include <string>
#include <sstream>
#include <algorithm>
#include <cstring>


#include <dirent.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>



#include "../src/DataTypes/Config.hpp"

using std::string;
using std::cout;
using std::endl;

namespace utils
{
	//	strings
	std::string&			replace(std::string& str, const std::string& old_word, const std::string& new_word);
	std::string&			replace_all(std::string& str, const std::string& old_word, const std::string& new_word);

	std::size_t				find_last_not_of(const std::string& s, const std::string& set, std::size_t start = 0);

	std::string&			trim_spaces(std::string& aString);
	std::string				trim_spaces(const std::string& aString);

	std::string&  			str_to_lower(std::string& str);

	// std::vector<string> 	SplitString(std::string line, char delimeter);

	unsigned int			string_to_uint(const std::string& str);

	template <typename T>
	std::string				to_string(T t)
	{
		std::ostringstream	ss;

		ss << t;
		return (ss.str());
	}

	//	network
	unsigned int			ip(unsigned char a, unsigned char b, unsigned char c, unsigned char d);

	unsigned int 			ip(const std::string& aIP);
	
	std::string				ip(unsigned int aIP);

	unsigned int 			hostname_to_ip_v4(const std::string& hostname);
	
	std::string				getExtension(string line);

	bool 					isValidIp_address(std::string ip_address);
	bool					IsDirectory(string uri);
	bool	 				isValidNumber(std::string line);

	int						get_exit_status(pid_t pid);
	int						stringToInt(std::string str);
	int 					min(int a, int b);
	
}

template<typename T>
std::ostream&	operator<<(std::ostream& o, const std::vector<T>& v)
{
	if (v.size())
	{
		o << "[ " << v[0];
		for (unsigned int i = 1; i < v.size(); ++i)
			o << ", "<< v[i] ;
		o << " ]";
	}
	else {
		std::cout << "[[ empty ]]" << std::endl;
	}
	return (o);
}

#endif
