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
#include "src/DataTypes/Config.hpp"
#include "src/Server/Server.hpp"

class ServerCluster
{
	public:
		ServerCluster();
		ServerCluster(Config *config);
		ServerCluster( const ServerCluster& s );
		~ServerCluster();

		ServerCluster&	operator=( const ServerCluster& s );

		bool		isRequestProperlyStructured(const IRequest &);
		bool		IsValidURI();
		void		SetupServers(Config* config);

		void		getMatchedServer(const IRequest &req);
		void		getMatchedRoute(const IRequest &req);
		bool		isServerMatched(const Server& , const  IRequest& );
	
		IResponse*  handle(IRequest* request);

	private:
		std::vector<Server *> servers;
		Server			*server;
		Route			*route;
		std::string		URI;
		unsigned int	statusCode;
		unsigned int	UriMaxlength;

};

#endif
