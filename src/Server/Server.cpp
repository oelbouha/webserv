/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysalmi <ysalmi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/19 18:45:58 by oelbouha          #+#    #+#             */
/*   Updated: 2024/03/17 13:50:21 by ysalmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include <iostream>

Server::Server(Config& serverConfig, ErrorPage& pages) :
	error_pages(pages),
	route(NULL)
{
	std::vector<std::string> raw_ports = serverConfig.getListConfigIfExist("ports");
	for (unsigned int i = 0; i < raw_ports.size(); ++i)
	{
		try
		{
			unsigned int	port = utils::string_to_uint(raw_ports[i]);
			if (port == 0 || port > 65535)
				throw std::invalid_argument("invalid port");
			ports.push_back(port);
		} catch (const std::exception& e)
		{
			throw ConfigException("Invalid value", "port", raw_ports[i]);
		}
	}

	is_default = serverConfig.getInlineConfigIfExist("default") == "yes";
	
	names = serverConfig.getListConfigIfExist("names");

	host = serverConfig.getInlineConfigOr("host", "0.0.0.0");//localhost, 12.hello.123.3

	ip = utils::hostname_to_ip_v4(host);
	if (ip == (unsigned int)-1)
		ip = utils::ip(host);
	
	host = utils::ip(ip);

	root = serverConfig.getInlineConfigIfExist("root");
	
	error_pages.setErrorPage(serverConfig.getBlockConfigIfExist("error_page"), root);

	std::vector<Config*> routeBlockConfig = serverConfig.getBlockConfig("route");
	std::vector<Config*>::iterator it = routeBlockConfig.begin();

	while (it != routeBlockConfig.end())
	{
		Config* routeConfig = *it;
	
		routeConfig->addInlineIfNotExist(serverConfig, "root");
		routeConfig->addInlineIfNotExist(serverConfig, "index");
		routeConfig->addInlineIfNotExist(serverConfig, "upload");

		routeConfig->addBlockIfExist(serverConfig, "error_page");
		routeConfig->addListIfExist(serverConfig, "cgi");

		Route *route = new Route(routeConfig, error_pages);
		routes.push_back(route);

		++it;
	}
}

const std::vector<unsigned int>& Server::getPorts() const  { return ports; }

unsigned int Server::getIP() const  { return ip; }

const std::vector<string>& Server::getNames() const  { return names; }

bool 	Server::isDefault() const { return is_default; }

Server::~Server()
{
	std::vector<Route*>::iterator it = routes.begin();
	while (it != routes.end())
	{
		delete *it;
		++it;
	}
}

Server&	Server::operator=( const Server& s )
{
    (void)s;
	return (*this);
}

bool	Server::findBestMatch(const string& reqURI, string routeURI)
{
	// if (reqURI == routeURI)
	// 	return true;
	if (strncmp(routeURI.c_str(), reqURI.c_str(), routeURI.length()) == 0)
		if (reqURI[routeURI.length()] == '/' || reqURI.length() == routeURI.length())
			return true;
    return (false);
}

Route*	Server::getMatchedRoute(const Request& req)
{
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

Result  Server::handle(Request& request)
{
	try {
		request.build();
		if (isRequestProperlyStructured(request) == false)
			return (Result(error_pages.build(request, statusCode)));

		route = getMatchedRoute(request);
		if (route == NULL)
			return (Result(error_pages.build(request, statusCode)));
		return (route->handle(request));
	} catch ( const RequestException& e ) {
		return Result(error_pages.build(request, e.error));
	}
}

bool	Server::isRequestProperlyStructured(const Request &req)
{
    const string& transfer_encoding  = req.getHeader("transfer-encoding");
    if (!transfer_encoding.empty() && transfer_encoding != "chunked")
    {
        Logger::debug ("request not well formed").flush();
        statusCode = 501;
        return false;
    }
    if (containsValidCharacters(req.getURI()) == false) {
        Logger::debug ("contains not valid characters").flush();
        statusCode = 400;
        return false;
    }
    else if (req.getURI().length() > MAX_URI_LENGTH) {
        statusCode = 414;
        return false;
    }
    return true;
}

bool	Server::containsValidCharacters(string uri)
{
    const string& validCharacters = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghij\
        klmnopqrstuvwxyz0123456789-._~:/?#[]@!$&'()*+,;=%";

    for (size_t i = 0; i < uri.length(); ++i)
    {
        if (validCharacters.find(uri[i]) == std::string::npos)
            return false;
    }
	return true;
}