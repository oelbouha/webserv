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

#include <iostream>

#include "src/Response/Response.hpp"
#include "src/Response/ProxiedResponse.hpp"
#include "src/Request/Request.hpp"
#include "src/DataTypes/Config.hpp"
#include "src/Server/Server.hpp"
#include "src/Server/ErrorPage.hpp"

class ServerCluster : public IHandler {
	std::vector<Server *>	servers;
	unsigned int			statusCode;
	unsigned int			UriMaxlength;
	public:
		ServerCluster();
		ServerCluster(Config *config);
		ServerCluster( const ServerCluster& s );
		ServerCluster&	operator=( const ServerCluster& s );
		~ServerCluster();

		bool			isServerMatched(const Server& , const  IRequest& );
		bool			isRequestProperlyStructured(const IRequest &);
		bool			containsValidCharacters(string uri);
		void			SetupServers(Config* config);

		string			getRoot() const ;
		unsigned int 	getStatusCode() const ;

		ErrorPage& 		getErrorPage() const;

		Server*			getDefaultServer();
		Server*			getMatchedServer(const IRequest &);
	
		IResponse*  	handle(const IRequest& request);

	private:
		Server		*server;
		ErrorPage	*error_pages;
		unsigned int KeepAlive;
};


/*
	nc ipaddres port 
	ipconfig getifaddr
*/

#endif
