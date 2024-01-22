/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelbouha <oelbouha@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/19 18:45:58 by oelbouha          #+#    #+#             */
/*   Updated: 2024/01/22 23:53:42 by oelbouha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include <iostream>

Server::Server(Config *serverConfig, ErrorPages& pages) : 
	error_pages(pages),
	route(NULL),
	redirectRoute(NULL),
	host("127.0.0.1"), 
	Default(false)
{
	if (serverConfig->hasInline("Default"))
		Default = true; 
	port = std::stod(serverConfig->getInlineConfig("port"), NULL);
	
	name = serverConfig->getInlineConfigIfExist("name");
	host = serverConfig->getInlineConfigIfExist("host");
	root = serverConfig->getInlineConfigIfExist("root");

	error_pages.setErrorPage(serverConfig, root);
	
	std::vector<Config*> routeBlockConfig = serverConfig->getBlockConfig("route");
	std::vector<Config*>::iterator it = routeBlockConfig.begin();
	
	while (it != routeBlockConfig.end()){
		Config* routeConfig = *it;
	
		routeConfig->addInLineIfExist(*serverConfig, "root");
		routeConfig->addInLineIfExist(*serverConfig, "index");
		routeConfig->addInLineIfExist(*serverConfig, "upload");

		routeConfig->addBlockIfExist(*serverConfig, "error_page");
		routeConfig->addBlockIfExist(*serverConfig, "cgi");

		Route *route = new Route(routeConfig, error_pages);
		routes.push_back(route);
		++it;
	}
}

// Server::Server( const Server& s ) {(void)s;}

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
	if (route->hasRedirection())
	{
		// redirect 
		// return ;
	}
	return (route->handle(request));
}