/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelbouha <oelbouha@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 21:59:54 by oelbouha          #+#    #+#             */
/*   Updated: 2024/05/18 13:17:57 by oelbouha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#ifndef SERVER_HPP
#define SERVER_HPP

#define MAX_URI_LENGTH 480

#include <iostream>
#include <dirent.h>

#include "src/Server/Result.hpp"
#include "src/Request/Request.hpp"
#include "src/DataTypes/Config.hpp"
#include "src/Route/Route.hpp"
#include "src/Server/ErrorPages.hpp"
#include "Utils.hpp"

#include "Logger.hpp"

class Route;

class Server
{
	std::vector<Route*>				routes;
	std::vector<unsigned int>		ports;
	std::vector<string>				names;
	ErrorPages						error_pages;
	Route*							route;
	unsigned int					ip;
	unsigned int					statusCode;
	std::string						host;
	std::string						root;
	std::string						rootPath;
	bool							is_default;
	
	bool			findBestMatch(const string& , string );
	Route*			getMatchedRoute(const Request& req);
	bool			isRequestProperlyStructured(const Request &);
	bool			containsValidCharacters(string uri);

	
public:
	Server(Config& config, ErrorPages& ErrorPages);
	Server&	operator=( const Server& s );
	~Server();

	const std::vector<unsigned int>&	getPorts() const;
	const std::vector<string>&			getNames() const;
	unsigned int 						getIP() const;
	const string&						getURI() const ;
	bool								isDefault() const;

	
	Result		handle(Request& );
};

#endif
