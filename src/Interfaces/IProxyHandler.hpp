/*	                     __          _
 *	   __  ___________ _/ /___ ___  (_)
 *	  / / / / ___/ __ `/ / __ `__ \/ /
 *	 / /_/ (__  ) /_/ / / / / / / / / 
 *	 \__, /____/\__,_/_/_/ /_/ /_/_/ 
 *	/____/	User: Youssef Salmi
 *			File: IProxyHandler.hpp 
 */

#pragma once
#ifndef ICGIHANDLER_HPP
#define ICGIHANDLER_HPP

#include "src/Interfaces/IProxiedResponse.hpp"
#include "src/Interfaces/IRequest.hpp"

class IProxyHandler
{
	public:
		virtual	~IProxyHandler(){};
        virtual IProxiedResponse*   handle(IRequest* request) = 0;
};

#endif
