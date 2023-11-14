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

namespace utils
{
	std::string&	replace_all(std::string& str, const std::string& old_word, const std::string& new_word);

	std::size_t		find_last_not_of(const std::string& s, const std::string& set, std::size_t start = 0);
}

#endif