/*	                     __          _
 *	   __  ___________ _/ /___ ___  (_)
 *	  / / / / ___/ __ `/ / __ `__ \/ /
 *	 / /_/ (__  ) /_/ / / / / / / / / 
 *	 \__, /____/\__,_/_/_/ /_/ /_/_/ 
 *	/____/	User: Youssef Salmi
 *			File: CGIHandler.hpp 
 */

#pragma once
#ifndef CGIHANDLER_HPP
#define CGIHANDLER_HPP

#include "IProxyHandler.hpp"
#include "src/Interfaces/IRequest.hpp"
#include "src/Response/ProxiedResponse.hpp"

#include "ProxyPair.hpp"
#include "DescriptorProxyRequest.hpp"
#include "ProxyResponse.hpp"

class CGIHandler
{
public:
	CGIHandler();
	CGIHandler( const CGIHandler& c);
	~CGIHandler();

	CGIHandler&	operator=( const CGIHandler& c );

    virtual ProxyPair*   handle(IRequest* request);
};
#endif
