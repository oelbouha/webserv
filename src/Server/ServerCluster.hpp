/*	                     __          _
 *	   __  ___________ _/ /___ ___  (_)
 *	  / / / / ___/ __ `/ / __ `__ \/ /
 *	 / /_/ (__  ) /_/ / / / / / / / / 
 *	 \__, /____/\__,_/_/_/ /_/ /_/_/ 
 *	/____/	User: Youssef Salmi
 *			File: ServerCluster.hpp 
 */

#pragma once
#ifndef SERVERCLUSTER_HPP
#define SERVERCLUSTER_HPP

#include <iostream>

#include "src/Response/Response.hpp"
#include "src/Response/ProxiedResponse.hpp"
#include "src/Request/Request.hpp"

class ServerCluster
{
public:
	ServerCluster();
	ServerCluster( const ServerCluster& s );
	~ServerCluster();

	ServerCluster&	operator=( const ServerCluster& s );

    IResponse*  handle(IRequest* request);
    IProxiedResponse*   handleCGI(IRequest* request);

};
#endif
