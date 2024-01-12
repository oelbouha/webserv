/*	                     __          _
 *	   __  ___________ _/ /___ ___  (_)
 *	  / / / / ___/ __ `/ / __ `__ \/ /
 *	 / /_/ (__  ) /_/ / / / / / / / / 
 *	 \__, /____/\__,_/_/_/ /_/ /_/_/ 
 *	/____/	User: Youssef Salmi
 *			File: ServerCluster.cpp 
 */

#include "ServerCluster.hpp"
#include <string>


ServerCluster::ServerCluster() {}

void    ServerCluster::SetupServers(Config* config){
    server = NULL;
    Config* cluster = config->getBlockConfig("cluster").front();
    std::vector<Config *> con = cluster->getBlockConfig("server");
    std::vector<Config *>::iterator it = con.begin();
    while (it != con.end()){
        Config  *con = *it;
        {
            // php .php
            // php7.4 .php
            // complete con;
            // Config* clusterCgiConfig = cluster->getBlockConfig("cgi").front();
            // std::vector<Config*> serverCgiConfig = con->getBlockConfig("cgi");

            // con->addBlock("cgi", clusterCgiConfig);
            // serverCgiConfig.push_back(clusterCgiConfig);
            // cgi
            // error pages
            // root            
        }
        Server *ser = new Server(con);
        servers.push_back(ser);
        ++it;
    }
}

ServerCluster::ServerCluster(Config* config){
    UriMaxlength = 2048;
    SetupServers(config);
}

bool	ServerCluster::IsValidURI(){
    string allowed_char = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-._~:/?#[]@!$&'()*+,;=%";
	std::string::iterator it = std::find_first_of(URI.begin(), URI.end()
		, allowed_char.begin(), allowed_char.end());
	if (it != URI.end())
		return true;
	return false;
}

ServerCluster::ServerCluster( const ServerCluster& s ) {(void)s;}

ServerCluster::~ServerCluster(){
    for(size_t i = 0; i < servers.size(); ++i)
        delete servers[i];
}

ServerCluster&	ServerCluster::operator=( const ServerCluster& s )
{
    (void)s;
	return (*this);
}

bool	ServerCluster::isRequestProperlyStructured(const IRequest &req)
{
	URI = req.getURI();
    try{
        std::string transfer  = req.getHeader("Transfer-Encoding");
        if (transfer != "chunked")
        {
            statusCode = 501;
            return false;
        }
    }
    catch(...){}

    if (IsValidURI() == false)
    {
        statusCode = 400;
        return false;
    }
    else if (URI.length() > UriMaxlength)
    {
        statusCode = 414;
        return false;
    }
    return true;
}


bool	ServerCluster::isServerMatched(const Server& server, const IRequest& req)
{
    unsigned int port = server.getPort();
    unsigned int inComingPort = req.getIncomingPort();
    unsigned int inComingIp = req.getIncomingIP();

    // std::cout << "port -> " << port << "incoming port -> " << inComingPort << std::endl;
    std::string host = req.getHeader("Host");
    int pos = host.find(":");
    host = host.substr(0, pos);
    if (host == server.getName())
        return true;
    (void)inComingIp;
    (void)port;
    (void)inComingPort;
    return false;
}

Server*	ServerCluster::getMatchedServer(const IRequest &req)
{
    std::vector<Server *>::iterator it = servers.begin();
    while (it != servers.end())
    {
        Server *server = *it;
        if (isServerMatched(*server, req)){
            return (server);
        }
        ++it;
    }
    it = servers.begin();
    while (it != servers.end()){
        Server *server = *it;
        if (server->isDefault()){
            return (server);
        }
        ++it;
    }
    return NULL;
}

void	ServerCluster::getMatchedRoute(const IRequest& req)
{
    URI = req.getURI();
    string uri;
    int pos = URI.rfind("/");
    if (pos >= 0)
        uri = URI.substr(0, pos + 1);
    // std::cout << "uri ->" << URI << pos <<  std::endl;
    // std::cout << "uri ->" << uri << std::endl;
    std::vector<Route*> routes = server->getRoutes();
    std::vector<Route*>::iterator it = routes.begin();
    while (it != routes.end()){
        Route *route = *it;
        if (route->getURI() == uri)
            server->setRoute(*route);
        ++it;
    }
    route = server->getMatchedRoute();
    if (route == NULL){
        std::vector<Route*>::iterator it = routes.begin();
        server->setRoute(*(*it)); // look for default
        return ;
    }
}

IResponse*  ServerCluster::handle(IRequest* request)
{
    if (isRequestProperlyStructured(*request) == false)
    {
        // the request is not well structerd;
    }
    server = getMatchedServer(*request);
    getMatchedRoute(*request);
    IResponse *res = server->handle(*request);
    return (res);
}

/*
IProxiedResponse*   ServerCluster::handleCGI(IRequest* request)
{
    CGIHandler    handler;

    return (handler.handle(request));
    std::string file = "pages/index.py";
    

    const std::string&  uri = request->getURI();
    if (uri != "/")
        file = "pages/" + uri.substr(uri.rfind("/"));

    ProxiedResponse& response = *new ProxiedResponse(request->getSocket());

    response.
}
*/
