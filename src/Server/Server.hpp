/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelbouha <oelbouha@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 21:59:54 by oelbouha          #+#    #+#             */
/*   Updated: 2024/02/06 22:14:20 by oelbouha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#ifndef SERVER_HPP
#define SERVER_HPP

#define MAX_URI_LENGTH 48

#include <iostream>
#include <dirent.h>

#include "src/Server/Result.hpp"
#include "src/Response/Response.hpp"
#include "src/Request/Request.hpp"
#include "src/DataTypes/Config.hpp"
#include "src/Route/Route.hpp"
#include "src/Server/ErrorPage.hpp"
#include "src/Interfaces/IServer.hpp"
#include "src/Interfaces/IHandler.hpp"
#include "Utils.hpp"

class Route;

class Server
{
	std::vector<Route*>				routes;
	std::vector<unsigned int>		ports;
	std::vector<string>				names;
	ErrorPage						error_pages;
	Route*							route;
	unsigned int					ip;
	unsigned int					statusCode;
	std::string						host;
	std::string						root;
	std::string						rootPath;
	bool							is_default;
	
	bool			findBestMatch(const string& , string );
	Route*			getMatchedRoute(const IRequest& req);

	bool			isRequestProperlyStructured(const IRequest &);
	bool			containsValidCharacters(string uri);

	
public:
	Server(Config& config, ErrorPage& errorPage);
	Server&	operator=( const Server& s );
	~Server();

	const std::vector<unsigned int>&	getPorts() const;
	const std::vector<string>&			getNames() const;
	unsigned int 						getIP() const;
	unsigned int 						getStatusCode() const ;
	const string&						getHost() const ;
	const string&						getRoot() const ;
	const string&						getURI() const ;
	bool								isDefault() const;
	const ErrorPage& 					getErrorPage() const;

	
	Result		handle(IRequest& );
};

#endif
