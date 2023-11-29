/*	                     __          _
 *	   __  ___________ _/ /___ ___  (_)
 *	  / / / / ___/ __ `/ / __ `__ \/ /
 *	 / /_/ (__  ) /_/ / / / / / / / / 
 *	 \__, /____/\__,_/_/_/ /_/ /_/_/ 
 *	/____/	User: Youssef Salmi
 *			File: ICGIHandler.hpp 
 */

#pragma once
#ifndef ICGIHANDLER_HPP
#define ICGIHANDLER_HPP

#include "IResponse.hpp"

class ICGIHandler
{
	public:
		virtual	~ICGIHandler(){};
        virtual IResponse   handle() = 0;
};

#endif