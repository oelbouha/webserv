/*	                     __          _
 *	   __  ___________ _/ /___ ___  (_)
 *	  / / / / ___/ __ `/ / __ `__ \/ /
 *	 / /_/ (__  ) /_/ / / / / / / / / 
 *	 \__, /____/\__,_/_/_/ /_/ /_/_/ 
 *	/____/	User: Youssef Salmi
 *			File: IConfigParser.hpp 
 */

#pragma once
#ifndef IPARSER_HPP
#define IPARSER_HPP

#include "Config.hpp"

class IConfigParser
{
	public:
		virtual		~IConfigParser(){};
		virtual const Config&	getConfig() = 0;

};
#endif
