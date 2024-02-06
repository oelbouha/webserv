/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelbouha <oelbouha@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 21:59:54 by oelbouha          #+#    #+#             */
<<<<<<< HEAD
/*   Updated: 2024/02/06 18:53:42 by oelbouha         ###   ########.fr       */
=======
/*   Updated: 2024/02/05 17:53:53 by ysalmi           ###   ########.fr       */
>>>>>>> 8a2965b142e51683d269029664b9d872939921bb
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
class RedirectRoute;

class Server : public IHandler
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
