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

#include <string>
#include <algorithm>

namespace utils
{
	std::string&	replace_all(std::string& str, const std::string& old_word, const std::string& new_word);

	std::size_t	find_last_not_of(const std::string& s, const std::string& set, std::size_t start = 0);

	unsigned int	ip(unsigned char a, unsigned char b, unsigned char c, unsigned char d);

	std::string	ip(unsigned int aIP);

	std::string&	trim_spaces(std::string& aString);

	std::string&  str_to_lower(std::string& str);
}

#endif
