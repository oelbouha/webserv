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

#include "src/DataTypes/Config.hpp"

class IConfigParser
{
	public:
		virtual		~IConfigParser(){};
		virtual  Config*	parse() = 0;
		virtual  Config&	getConfig() = 0;

};
#endif
