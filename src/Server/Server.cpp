

#include "Server.hpp"

Server::Server(Config *serverConfig){
	route = NULL;
	Default = true; // set it to false when it added to config
	name = serverConfig->getInlineConfig("name");
	port = std::stod(serverConfig->getInlineConfig("port"), NULL);
	std::vector<Config*> routeBlockConfig = serverConfig->getBlockConfig("route");
	std::vector<Config*>::iterator it = routeBlockConfig.begin();
	std::string	prop;
	while (it != routeBlockConfig.end()){
		Config* routeConfig = *it;
		{
			// addIfNotExist(routeConfig, serverConfig, "root");
			// addIfNotExist(routeConfig, serverConfig, "index");
			if (routeConfig->hasInline("root") == false){
				prop = serverConfig->getInlineConfig("root");
				routeConfig->addInline("root", prop);
			}
			if (routeConfig->hasInline("index") == false){
				prop = serverConfig->getInlineConfig("index");
				routeConfig->addInline("index", prop);
			}
		}
		Route *route = new Route(routeConfig);
		routes.push_back(route);
		++it;
	}
}

Server::Server( const Server& s ) {(void)s;}

unsigned int Server::getPort() const  { return port; }

unsigned int Server::getIp() const  { return ip; }

std::string Server::getName() const  { return name; }

std::string Server::getRoot() const  { return root; }

std::vector<Route*> Server::getRoutes() const { return routes; };

void 	Server::setIp(unsigned int ip)  { this->ip = ip; }

void	Server::setRoute(Route& route) { this->route = &route; }

bool 	Server::isDefault() const { return Default; }

void 	Server::setPort(unsigned int port)  { this->port = port; }

Server::~Server() {}

Server&	Server::operator=( const Server& s ){
    (void)s;
	return (*this);
}

bool	Server::IsRouteURIMatched(const string& reqURI, const string& routeURI){
	// std::cout << "req uri :" << reqURI << ": | route uri :" << routeURI << ":" << std::endl;
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
	if (route)
		return (route->handle(request));
	std::cout << "there is no route for :" << request.getURI() << std::endl;;
	IResponse* response = new Response(request.getSocket());
	response->setStatusCode(404)
		.setHeader("content-type", "text/html")
		.setHeader("connection", "close")
		.setBody("404 Not Found")
		.build();
	return response;

}