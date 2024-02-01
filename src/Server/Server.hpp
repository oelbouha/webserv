/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysalmi <ysalmi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 21:59:54 by oelbouha          #+#    #+#             */
/*   Updated: 2024/02/01 09:29:18 by ysalmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
#include "src/Server/Helper.hpp"
#include "src/Server/ErrorPage.hpp"
#include "src/Route/RedirectRoute.hpp"
#include "src/Interfaces/IServer.hpp"
#include "Utils.hpp"

class Route;
class RedirectRoute;

class Server : public IHandler {
std::vector<Route*>	routes;
std::vector<string>	ports;
std::vector<string>	names;
public:
	Server(Config * config, ErrorPage& errorPage);
	const Config*	getConfig() const;
	Server&	operator=( const Server& s );
	~Server();

	std::vector<string> getPort() const;
	std::vector<string>	getName() const ;

	unsigned int getIp() const;
	unsigned int getStatusCode() const ;

	string	getHost() const ;
	string	getRoot() const ;
	string	getURI() const ;

	bool	findBestMatch(const string& , string );
	bool	isDefault() const;
	
	Route*		getMatchedRoute(const IRequest& req);
	IResponse*	handle(IRequest& );
	ErrorPage& 	getErrorPage() const;

private:
	ErrorPage		&error_pages;
	Route			*route;
	unsigned int	ip;
	unsigned int	statusCode;
	std::string		host;
	std::string		root;
	std::string		rootPath;
	std::string		Default;
};

#endif
