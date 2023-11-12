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
}

#endif