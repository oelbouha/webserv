/*	                     __          _
 *	   __  ___________ _/ /___ ___  (_)
 *	  / / / / ___/ __ `/ / __ `__ \/ /
 *	 / /_/ (__  ) /_/ / / / / / / / / 
 *	 \__, /____/\__,_/_/_/ /_/ /_/_/ 
 *	/____/	User: Youssef Salmi
 *			File: ServersCluster.hpp 
 */

#pragma once
#ifndef SERVERSCLUSTER_HPP
#define SERVERSCLUSTER_HPP

#include <iostream>
#include <string>
#include <map>

#include "IServer.hpp"

#include "Socket.hpp"

#include "Types.hpp"

class ServersCluster
{
	std::map<std::string, IServer>	mServers;

	public:
		ServersCluster();
		ServersCluster( ServersCluster& s );
		~ServersCluster();

		ServersCluster&	operator=( ServersCluster& s );
		IServer&		operator[]( int idx );

		IServer&		findMatch(int aPort, ip_t aIP = 0, const std::string& aName = "");

};
#endif