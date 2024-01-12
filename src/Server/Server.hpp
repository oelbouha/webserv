

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

class Route;

class Server
{
	public:
		Server(Config * config);
		Server( const Server& s );
		Server&	operator=( const Server& s );
		~Server();

		unsigned int getPort() const;
		unsigned int getIp() const;

		std::string	getName() const ;
		std::string	getRoot() const ;
		std::vector<Route*> getRoutes() const ;
	
		bool	isDefault() const;
		void	setPort( unsigned int );
		void	setRoute(Route& route);
		void	setIp( unsigned int );
		void	setName( string name );
		
		IResponse*	handle(const IRequest& );
		Route* 		getMatchedRoute() const;

		const Config*		getConfig() const;

	private:
		std::vector<Route*>	routes;
		Route				*route;
		unsigned int		port;
		unsigned int		ip;
		std::string			name;
		std::string			root;
		bool				Default;
};

#endif
