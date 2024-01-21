

#pragma once
#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>

#include <dirent.h>
#include "src/Response/Response.hpp"
#include "src/Response/ProxiedResponse.hpp"
#include "src/Request/Request.hpp"
#include "src/DataTypes/Config.hpp"
#include "src/Route/Route.hpp"
#include "src/Interfaces/IServer.hpp"
#include "Utils.hpp"

class Route;

class Server : public IServer {
	std::vector<Route*>	routes;

	public:
		Server(Config * config);
		Server( const Server& s );
		const Config*	getConfig() const;
		Server&	operator=( const Server& s );
		~Server();

		unsigned int getPort() const;
		unsigned int getIp() const;

		string	getName() const ;
		string	getHost() const ;
		string	getRoot() const ;
		string	getURI() const ;

		bool	IsRouteURIMatched(const string& , const string& );
		bool	isDefault() const;
		
		IResponse*	handle(const IRequest& );
		Route*		getMatchedRoute(const IRequest& req);

	private:
		Route			*route;
		unsigned int	port;
		unsigned int	ip;
		std::string		name;
		std::string		host;
		std::string		root;
		std::string		rootPath;
		bool			Default;
};

#endif
