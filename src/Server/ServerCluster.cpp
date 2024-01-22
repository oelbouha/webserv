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

    error_page  = cluster->getBlockConfigIfExist("error_page");
    setupErrorPages();
    
    std::vector<Config *> ServersConfig = cluster->getBlockConfigIfExist("server");
    std::vector<Config *>::iterator it = ServersConfig.begin();
    
    while (it != ServersConfig.end()){
        Config  *serverConfig = *it;

        serverConfig->addBlockIfExist(*cluster, "error_page");
		serverConfig->addListIfExist(*cluster, "cgi");

        Server *server = new Server(serverConfig, errorPages);
        servers.push_back(server);
        ++it;
    }
}

std::string	ServerCluster::getRoot() const{ return root; }

ServerCluster::ServerCluster(Config* config){
    UriMaxlength = 20;
    SetupServers(config);
}

bool	ServerCluster::IsValidURI(string uri){
    const string& allowed_char = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-._~:/?#[]@!$&'()*+,;=%";
	std::string::iterator it = std::find_first_of(uri.begin(), uri.end()
		, allowed_char.begin(), allowed_char.end());
    if (it != uri.end())
		return true;
	return false;
}

ServerCluster::ServerCluster( const ServerCluster& s ) {(void)s;}

ServerCluster::~ServerCluster(){
    for(size_t i = 0; i < servers.size(); ++i)
        delete servers[i];
}

ServerCluster&	ServerCluster::operator=( const ServerCluster& s ){
    (void)s;
	return (*this);
}

bool	ServerCluster::isRequestProperlyStructured(const IRequest &req)
{
    try{
        const string& transfer  = req.getHeader("Transfer-Encoding");
        if (transfer != "chunked")
        {
            statusCode = 501;
            return false;
        }
    }
    catch(...){}

    if (IsValidURI(req.getURI()) == false)
    {
        statusCode = 400;
        return false;
    }
    else if (req.getURI().length() > UriMaxlength)
    {
        statusCode = 414;
        return false;
    }
    return true;
}

bool	ServerCluster::isServerMatched(const Server& server, const IRequest& req){
    unsigned int serverPort = server.getPort();
    const string& serverHost = server.getHost();

    unsigned int inComingPort = req.getIncomingPort();
    const string& inComingHost = utils::ip(req.getIncomingIP());
    // if (serverPort == inComingPort)
    //     std::cout << "port matched\n";
    string host = req.getHeader("Host");
    int pos = host.find(":");
    host = host.substr(0, pos);
    if (host == server.getName())
        return true;

    (void)serverPort;
    (void)inComingPort;
    (void)inComingHost;
    (void)serverHost;
    return false;
}

Server*	ServerCluster::getDefaultServer(){
    std::vector<Server *>::iterator it = servers.begin();
    while (it != servers.end()){
        Server *server = *it;
        if (server->isDefault())
            return (server);
        ++it;
    }
    return (servers[0]);
}

Server*	ServerCluster::getMatchedServer(const IRequest &req)
{
    std::vector<Server *>::iterator it = servers.begin();
    while (it != servers.end()){
        Server *server = *it;
        if (isServerMatched(*server, req)){
            return (server);
        }
        ++it;
    }
    return (getDefaultServer());
}

IResponse*  ServerCluster::handle(const IRequest& request)
{
    server = getMatchedServer(request);
    if (isRequestProperlyStructured(request) == false)
    {
        root = server->getURI() + request.getURI();
        return (GenerateErrorPageResponse(request, statusCode));
    }
    return (server->handle(request));
}

