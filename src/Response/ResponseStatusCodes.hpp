/*	                     __          _
 *	   __  ___________ _/ /___ ___  (_)
 *	  / / / / ___/ __ `/ / __ `__ \/ /
 *	 / /_/ (__  ) /_/ / / / / / / / / 
 *	 \__, /____/\__,_/_/_/ /_/ /_/_/ 
 *	/____/	User: Youssef Salmi
 *			File: ResponseStatusCodes.hpp 
 */

#pragma once
#ifndef RESPONSESTATUSCODES_HPP
#define RESPONSESTATUSCODES_HPP

#include <iostream>

class ResponseStatusCodes
{
	static	 
	ResponseStatusCodes();
public:
	static const std::string& get(unsigned int aCode);

};
#endif
