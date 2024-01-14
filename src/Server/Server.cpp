

#include "Server.hpp"

Server::Server(Config *config){
	route = NULL;
	Default = true; // set it to false when it added to config
	name = config->getInlineConfig("name");
	
	port = std::stod(config->getInlineConfig("port"), NULL);
	// set up Ip whene it added to config
	std::vector<Config*> r = config->getBlockConfig("route");
	std::vector<Config*>::iterator it = r.begin();
	while (it != r.end()){
		Config* con = *it;
		{
			string root = config->getInlineConfig("root");
			con->addInline("root", root);
			root = config->getInlineConfig("index");
			con->addInline("index", root);
		}
		Route *r = new Route(con);
		routes.push_back(r);
		++it;
	}
}

Server::Server( const Server& s ) {(void)s;}

unsigned int Server::getPort() const  { return port; }

unsigned int Server::getIp() const  { return ip; }

std::string Server::getName() const  { return name; }

std::string Server::getRoot() const  { return root; }

std::vector<Route*> Server::getRoutes() const { return routes; };

Route*	Server::getMatchedRoute() const { return route; };

void 	Server::setIp(unsigned int ip)  { this->ip = ip; }

void	Server::setRoute(Route& route) { this->route = &route; }

bool 	Server::isDefault() const { return Default; }

void 	Server::setPort(unsigned int port)  { this->port = port; }

Server::~Server() {}

Server&	Server::operator=( const Server& s ){
    (void)s;
	return (*this);
}

IResponse*  Server::handle(const IRequest& request){
	return (route->handle(request));
}