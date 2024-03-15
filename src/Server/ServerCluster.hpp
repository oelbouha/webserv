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
#include <cstring>



#include "src/Server/Server.hpp"
#include "src/Server/Result.hpp"
#include "src/Server/ErrorPage.hpp"
#include "src/Request/Request.hpp"
#include "src/CGI/ProxyPair.hpp"
#include "src/CGI/CGIResponse.hpp"
#include "src/CGI/CGIRequest.hpp"
#include "src/DataTypes/Config.hpp"




class ServerCluster
{
	std::vector<Server *>	servers;
	ErrorPage				error_pages;

	ServerCluster( const ServerCluster& s );
	ServerCluster&	operator=( const ServerCluster& s );

	bool			isServerMatched(const Server&, unsigned int ip, unsigned int port);
	void			SetupServers(Config* config);
	Server*			getMatchedServer(const Request &);

public:
	ServerCluster(Config *config);
	~ServerCluster();

	Result  			handle(Request& request);

	std::vector<std::pair<unsigned int, unsigned int> >	getServersIPPortPairs() const;
};

#endif
