/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelbouha <oelbouha@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/19 18:45:58 by oelbouha          #+#    #+#             */
/*   Updated: 2024/01/20 21:25:55 by oelbouha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include <iostream>

Server::Server(Config *serverConfig) : host("127.0.0.1"){
	route = NULL;
	Default = false;
	if (serverConfig->hasInline("Default"))
		Default = true; 
	port = std::stod(serverConfig->getInlineConfig("port"), NULL);
	
	setInlineIfExist(*serverConfig, name, "name");
	setInlineIfExist(*serverConfig, host, "host");
	setInlineIfExist(*serverConfig, root, "root");
	setBlockIfExist(*serverConfig, error_page, "error_page");
	
	std::vector<Config*> routeBlockConfig = serverConfig->getBlockConfig("route");
	std::vector<Config*>::iterator it = routeBlockConfig.begin();
	
	while (it != routeBlockConfig.end()){
		Config* routeConfig = *it;
	
		addInlineIfNotExist(*routeConfig, *serverConfig, "root");
		addInlineIfNotExist(*routeConfig, *serverConfig, "index");
		addInlineIfNotExist(*routeConfig, *serverConfig, "upload");
		
		addBlockIfExist(*routeConfig, *serverConfig, "error_page");
		addBlockIfExist(*routeConfig, *serverConfig, "cgi");

		Route *route = new Route(routeConfig);
		routes.push_back(route);
		++it;
	}
	setupErrorPages();
}

Server::Server( const Server& s ) {(void)s;}

unsigned int Server::getPort() const  { return port; }

unsigned int Server::getIp() const  { return ip; }

string Server::getName() const  { return name; }

string Server::getRoot() const  { return rootPath; }

string Server::getURI() const  { return root; }

string Server::getHost() const  { return host; }

bool 	Server::isDefault() const { return Default; }


Server::~Server() {}

Server&	Server::operator=( const Server& s ){
    (void)s;
	return (*this);
}

bool	Server::IsRouteURIMatched(const string& reqURI, const string& routeURI){
	if (strncmp(routeURI.c_str(), reqURI.c_str(), routeURI.length()) == 0)
		return true;
    return (false);
}

Route*	Server::getMatchedRoute(const IRequest& req)
{
	Route* ret = NULL;
    const string& reqUri = req.getURI();
    std::vector<Route*>::iterator it = routes.begin();
    while (it != routes.end()){
        Route *route = *it;
        if (IsRouteURIMatched(reqUri, route->getURI()))
        {
			if (ret == NULL)
				ret = route;
			else if (ret && ret->getURI().length() < route->getURI().length())
				ret = route;
		}
        ++it;
    }
    return (ret);
}

IResponse*  Server::handle(const IRequest& request){
	route = getMatchedRoute(request);
	if (route == NULL)
	{
		std::cout << "No route matched uri ......\n";
		rootPath = root + request.getURI();
		return (GenerateErrorPageResponse(request, 404));
	}
	return (route->handle(request));
}