/*	                     __          _
 *	   __  ___________ _/ /___ ___  (_)
 *	  / / / / ___/ __ `/ / __ `__ \/ /
 *	 / /_/ (__  ) /_/ / / / / / / / / 
 *	 \__, /____/\__,_/_/_/ /_/ /_/_/ 
 *	/____/	User: Youssef Salmi
 *			File: ServerCluster.hpp 
 */

#ifndef SERVERCLUSTER_HPP
#define SERVERCLUSTER_HPP


#define CLUSTER_DEFAULT_ROOT "/tmp"

#include <iostream>
#include <utility>

// #include "src/Response/Response.hpp"
#include "src/Request/Request.hpp"

#include "src/CGI/ProxyPair.hpp"

#include "src/CGI/CGIResponse.hpp"
#include "src/CGI/CGIRequest.hpp"

#include "src/DataTypes/Config.hpp"
#include "src/Server/Server.hpp"
#include "src/Server/ErrorPage.hpp"

#include "Result.hpp"

#include <cstring>


class ServerCluster
{
	std::vector<Server *>	servers;
	ErrorPage				error_pages;
	unsigned int 			KeepAlive;

	ServerCluster( const ServerCluster& s );
	ServerCluster&	operator=( const ServerCluster& s );

	bool			isServerMatched(const Server&, unsigned int ip, unsigned int port);
	void			SetupServers(Config* config);
	Server*			getMatchedServer(const IRequest &);

public:
	ServerCluster(Config *config);
	~ServerCluster();

	Result  			handle(IRequest& request);

	std::vector<std::pair<unsigned int, unsigned int> >	getServersIPPortPairs() const;
};

#endif
