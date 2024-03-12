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
	//	Strings 
	bool			is_not_space(char c)
	{
		return (std::isspace(c) == false);
	}

	std::string&	replace(std::string& str, const std::string& old_word, const std::string& new_word)
	{
		std::size_t n = str.find(old_word);
		if (n != std::string::npos)
			str.replace( n, old_word.length(), new_word );
		return (str);
	}

	std::string&	replace_all(std::string& str, const std::string& old_word, const std::string& new_word)
	{
		std::size_t n = 0;
		while ( ( n = str.find( old_word, n ) ) != std::string::npos )
		{
			str.replace( n, old_word.length(), new_word );
			n += old_word.length();
		}
		return (str);
	}

	std::size_t		find_last_not_of(const std::string& s, const std::string& set, std::size_t start)
	{
		std::size_t pos = s.length() - 1;

		while (pos != std::string::npos && set.find(s[pos]) != std::string::npos)
			--pos;
		++pos;
		if (pos >= start)
			return (pos);
		return (std::string::npos);
	}

	std::string&	trim_spaces(std::string& s)
	{
		s.erase(s.begin(), 
			std::find_if( s.begin(), s.end(), is_not_space ));
		s.erase( std::find_if( s.rbegin(), s.rend(), is_not_space ).base(), 
			s.end());
		return (s);
	}
	
	std::string		trim_spaces(const std::string& str)
	{
		std::string	s(str);
		return (trim_spaces(s));
	}

	std::string&	str_to_lower(std::string& str)
	{
		for (std::string::iterator it = str.begin(); it != str.end(); ++it)
			*it = std::tolower(*it);
		return (str);
	}

	char** 			vector_to_cstring_array(const std::vector<std::string>& vec)
	{
		char**  ret = new char*[vec.size() + 1];
		int     k = 0;
		for (std::vector<std::string>::const_iterator it = vec.begin(); it != vec.end(); ++it)
		{
			char*   str = new char[it->length() + 1];
			for (size_t i = 0; i < it->length(); ++i)
				str[i] = static_cast<char>(it->at(i));
			str[it->length()] = 0;
			ret[k++] = str;
		}
		ret[k] = NULL;
		return (ret);
	}

	std::string  	decode_uri(const string& uri)
	{
		std::string decoded = uri;
		size_t pos = 0;
		while (true) {
			pos = decoded.find("%", pos);
			if (pos == std::string::npos)
				break;
			char c = static_cast<char>(hex_to_uint(decoded.substr(pos+1, 2)));
			decoded.erase(pos, 2);
			decoded[pos] = c;
		}
		return decoded;
	}

	//	Numbers
	unsigned int	hex_to_uint(const std::string& str)
	{
		unsigned int        x;   
		std::stringstream   ss;

		ss << std::hex << str;
		ss >> x;
		return (x);
	}
	
	unsigned int	string_to_uint(const std::string& str)
	{
		std::string			trimmed = trim_spaces(str);
		std::stringstream	ss(trimmed);
		long unsigned int	res;
		std::string			rest;

		ss >> res;
		ss >> rest;
		if (! rest.empty() || !std::isdigit(trimmed[0]) || res > UINT_MAX)
			throw std::invalid_argument("argument is not an unsigned integer value");
		return (res);
	}
	
	//	Network
	unsigned int	ip(unsigned char a, unsigned char b, unsigned char c, unsigned char d)
	{
		return (a << 24 | b << 16 | c << 8 | d);
	}

	std::string		ip(unsigned int aIP)
	{
		std::ostringstream	ss;
		unsigned int		byte;

		byte = (aIP >> 24) & 255;
		ss << byte << ".";
		byte = (aIP >> 16) & 255;
		ss << byte << ".";
		byte = (aIP >> 8) & 255;
		ss << byte << ".";
		byte = aIP & 255;
		ss << byte;
		return (ss.str());
	}
	
	unsigned int	ip(const std::string& ip_address)
	{
		std::istringstream	ss(ip_address);
		std::vector<int>	components;
		std::string			component;

		while (std::getline(ss, component, '.')){
			components.push_back(std::stod(component, NULL));
		}

		if (components.size() != 4)
			return -1;

		return ip(components[0], components[1], components[2], components[3]);
	}

	unsigned int	hostname_to_ip_v4(const std::string& hostname)
	{
		unsigned int		ret = -1;
		addrinfo 	        hints, *result;

		std::memset(&hints, 0, sizeof(hints));
		hints.ai_family = AF_INET;
		hints.ai_socktype = SOCK_STREAM;

		int	err = getaddrinfo(hostname.c_str(), NULL, &hints, &result);
		if (err != 0)
			return (-1);

		if (result)
		{
			sockaddr_in& addr_in = *reinterpret_cast<sockaddr_in *>(result->ai_addr);
			ret = addr_in.sin_addr.s_addr;
		}
		freeaddrinfo(result);
		return ret;
	}
	
	//	Other
	int		get_exit_status(pid_t pid)
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

	bool	is_directory(string uri)
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

	bool	is_valid_number(std::string line)
	{
		for(size_t i = 0; i < line.length(); ++i) {
			if (std::isdigit(line[i]) == false)
				return false;
		}
		return true;
	}

	bool	is_valid_ip_address(std::string ip_address)
	{
		if (ip_address == "localhost")
			return true;
		std::istringstream ss(ip_address);
		std::string component;
		unsigned int count = 0;

		while (std::getline(ss, component, '.')) {
			if (is_valid_number(component) == false) {
				return 0;
			}
			++count;
		}
		return (count == 4);
	}

	int		min(int a, int b)
	{
		return (a < b) ? a : b;
	}

	
}


