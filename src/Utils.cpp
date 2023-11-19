/*	                     __          _
 *	   __  ___________ _/ /___ ___  (_)
 *	  / / / / ___/ __ `/ / __ `__ \/ /
 *	 / /_/ (__  ) /_/ / / / / / / / / 
 *	 \__, /____/\__,_/_/_/ /_/ /_/_/ 
 *	/____/	User: Youssef Salmi
 *			File: Utils.cpp 
 */

#include "Utils.hpp"

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

        while (pos >= 0 && set.find(s[pos]) != std::string::npos)
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
		unsigned char	byte = aIP >> 24;

		ret += std::to_string(byte) + ".";
		byte = aIP >> 16 & 255;
		ret += std::to_string(byte) + ".";
		byte = aIP >> 8 & 255;
		ret += std::to_string(byte) + ".";
		byte = aIP & 255;
		ret += std::to_string(byte);
		return (ret);
	}
}