/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelbouha <oelbouha@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/19 18:45:58 by oelbouha          #+#    #+#             */
/*   Updated: 2024/01/24 11:52:23 by oelbouha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include <iostream>

Server::Server(Config *serverConfig, ErrorPage& pages) : 
		error_pages(pages), route(NULL), host("127.0.0.1") {

	port = utils::stringToInt(serverConfig->getInlineConfigIfExist("port"));
	
	Default = serverConfig->getInlineConfigIfExist("Default");
	name = serverConfig->getInlineConfigIfExist("name");
	host = serverConfig->getInlineConfigIfExist("host");
	root = serverConfig->getInlineConfigIfExist("root");

	error_pages.setErrorPage(*serverConfig);
	
	std::vector<Config*> routeBlockConfig = serverConfig->getBlockConfig("route");
	std::vector<Config*>::iterator it = routeBlockConfig.begin();
	
	while (it != routeBlockConfig.end()){
		Config* routeConfig = *it;
	
		routeConfig->addInlineIfNotExist(*serverConfig, "root");
		routeConfig->addInlineIfNotExist(*serverConfig, "index");
		routeConfig->addInlineIfNotExist(*serverConfig, "upload");

		routeConfig->addBlockIfExist(*serverConfig, "error_page");
		routeConfig->addBlockIfExist(*serverConfig, "cgi");

		Route *route = new Route(routeConfig, error_pages);
		routes.push_back(route);
		++it;
	}
}

// Server::Server( const Server& s ) {(void)s;}

unsigned int Server::getPort() const  { return port; }

unsigned int Server::getStatusCode() const  { return statusCode; }

unsigned int Server::getIp() const  { return ip; }

string Server::getName() const  { return name; }

string Server::getRoot() const  { return root; }

string Server::getHost() const  { return host; }

ErrorPage& 	Server::getErrorPage() const { return error_pages; }

bool 	Server::isDefault() const { return Default.size(); }

Server::~Server() {}

Server&	Server::operator=( const Server& s ){
    (void)s;
	return (*this);
}

bool	Server::IsRouteURIMatched(const string& reqURI, const string& routeURI){
	if (strncmp(routeURI.c_str(), reqURI.c_str(), routeURI.length()) == 0)
	{
		if(reqURI[routeURI.length()] == '\0' || reqURI[routeURI.length()] == '/')
			return true;
	}
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
		std::cout << "No route matched uri ... \n";
		statusCode = 404;
		return Helper::BuildResponse(request, *this);
	}
	else if (route->hasRedirection())
	{
		RedirectRoute redirect(*route, error_pages);
		return (redirect.handle(request));
	}
	return (route->handle(request));
}