/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelbouha <oelbouha@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/19 18:45:58 by oelbouha          #+#    #+#             */
/*   Updated: 2024/01/30 15:04:26 by oelbouha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include <iostream>

Server::Server(Config *serverConfig, ErrorPage& pages) : error_pages(pages), route(NULL) {

	std::string port = serverConfig->getInlineConfigIfExist("port");
	ports = utils::SplitString(port, ' ');

	Default = serverConfig->getInlineConfigIfExist("Default");
	
	std::string name = serverConfig->getInlineConfigIfExist("name");
	names = utils::SplitString(name, ' ');

	host = serverConfig->getInlineConfigIfExist("host");
	root = serverConfig->getInlineConfigIfExist("root");

	error_pages.setErrorPage(*serverConfig);

	std::vector<Config*> routeBlockConfig = serverConfig->getBlockConfig("route");
	std::vector<Config*>::iterator it = routeBlockConfig.begin();

	while (it != routeBlockConfig.end()) {
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

std::vector<string> Server::getPort() const  { return ports; }

unsigned int Server::getStatusCode() const  { return statusCode; }

unsigned int Server::getIp() const  { return ip; }

std::vector<string> Server::getName() const  { return names; }

string Server::getRoot() const  { return root; }

string Server::getHost() const  { return host; }

ErrorPage& 	Server::getErrorPage() const { return error_pages; }

bool 	Server::isDefault() const { return Default.size(); }

Server::~Server() {
	std::vector<Route*>::iterator it = routes.begin();
	while (it != routes.end()) {
		delete *it;
		++it;
	}
}

Server&	Server::operator=( const Server& s ) {
    (void)s;
	return (*this);
}

bool	Server::findBestMatch(const string& reqURI, string routeURI) {
	int length = utils::min(reqURI.length(), routeURI.length());
	if (strncmp(routeURI.c_str(), reqURI.c_str(), length) == 0) {
		if (routeURI[length] == 0)
			return true;
	}
    return (false);
}

Route*	Server::getMatchedRoute(const IRequest& req) {
	Route* ret = NULL;
    const string& reqUri = req.getURI();
    std::vector<Route*>::iterator it = routes.begin();
    while (it != routes.end()){
        Route *route = *it;
        if (findBestMatch(reqUri, route->getURI())) {
			if (ret == NULL)
				ret = route;
			else if (ret && ret->getURI().length() < route->getURI().length())
				ret = route;
		}
        ++it;
    }
    return (ret);
}

IResponse*  Server::handle(const IRequest& request) {
	route = getMatchedRoute(request);
	if (route)
		std::cout << "matched route  <" << route->getURI() << ">" << std::endl;
	if (route == NULL)
	{
		std::cout << "No route matched uri ... <" << request.getURI() << ">" << std::endl;;
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