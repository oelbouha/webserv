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
#include <map>

#include "IServer.hpp"

class ServersCluster
{
	/*	Private members here	*/
	std::map<int, IServer>	mServers;
	int						mMinFd;
	int						mMaxFd;

	public:
		ServersCluster( void );
		ServersCluster( ServersCluster& s );
		~ServersCluster();

		ServersCluster&	operator=( ServersCluster& s );
		IServer&		operator[]( int idx );

};
#endif