/*	                     __          _
 *	   __  ___________ _/ /___ ___  (_)
 *	  / / / / ___/ __ `/ / __ `__ \/ /
 *	 / /_/ (__  ) /_/ / / / / / / / / 
 *	 \__, /____/\__,_/_/_/ /_/ /_/_/ 
 *	/____/	User: Youssef Salmi
 *			File: Cluster.hpp 
 */

#ifndef Cluster_HPP
#define Cluster_HPP


#define CLUSTER_DEFAULT_ROOT "/tmp"

#include <iostream>
#include <utility>
#include <cstring>
#include <set>



#include "src/Server/Server.hpp"
#include "src/Server/Result.hpp"
#include "src/Server/ErrorPages.hpp"
#include "src/Request/Request.hpp"
#include "src/CGI/ProxyPair.hpp"
#include "src/CGI/CGIResponse.hpp"
#include "src/CGI/CGIRequest.hpp"
#include "src/DataTypes/Config.hpp"




class Cluster
{
	std::vector<Server *>	servers;
	ErrorPages				error_pages;

	Cluster( const Cluster& s );
	Cluster&	operator=( const Cluster& s );

	bool			isServerMatched(const Server&, unsigned int ip, unsigned int port);
	void			SetupServers(Config* config);
	Server*			getMatchedServer(Request &);

public:
	Cluster(Config *config);
	~Cluster();

	Result  			handle(Request& request);

	std::set<std::pair<unsigned int, unsigned int> >	getServersIPPortPairs() const;
};

#endif
