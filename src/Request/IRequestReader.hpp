/*	                     __          _
 *	   __  ___________ _/ /___ ___  (_)
 *	  / / / / ___/ __ `/ / __ `__ \/ /
 *	 / /_/ (__  ) /_/ / / / / / / / / 
 *	 \__, /____/\__,_/_/_/ /_/ /_/_/ 
 *	/____/	User: Youssef Salmi
 *			File: IRequestReader.hpp 
 */

#pragma once
#ifndef IREQUESTREADER_HPP
#define IREQUESTREADER_HPP

#include <string>

class IRequestReader
{
public:
	virtual ~IRequestReader(){};
	virtual	std::string	read() = 0;
	virtual bool		eof() const = 0;
    virtual size_t      getContentLength() const = 0;

};
#endif
