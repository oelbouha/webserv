/*	                     __          _
 *	   __  ___________ _/ /___ ___  (_)
 *	  / / / / ___/ __ `/ / __ `__ \/ /
 *	 / /_/ (__  ) /_/ / / / / / / / / 
 *	 \__, /____/\__,_/_/_/ /_/ /_/_/ 
 *	/____/	User: Youssef Salmi
 *			File: ServerCluster.cpp 
 */

#include "ServerCluster.hpp"


ServerCluster::ServerCluster( const ServerCluster& s ) {(void)s;}

ServerCluster&	ServerCluster::operator=( const ServerCluster& s )
{
    (void)s;
	return (*this);
}


ServerCluster::ServerCluster(Config* cluster)
{    
    std::string root = cluster->getInlineConfigOr("root", CLUSTER_DEFAULT_ROOT);
    error_pages.setErrorPage(cluster->getBlockConfigIfExist("error_page"), root);

    std::vector<Config *> ServersConfig = cluster->getBlockConfigIfExist("server");
    std::vector<Config *>::iterator it = ServersConfig.begin();

    while (it != ServersConfig.end())
    {
        Config&  serverConfig = **it;

		serverConfig.addListIfExist(*cluster, "cgi");
        serverConfig.addInlineIfNotExist(*cluster, "root");

        servers.push_back(new Server(serverConfig, error_pages));
        ++it;
    }
}

ServerCluster::~ServerCluster()
{
    for(size_t i = 0; i < servers.size(); ++i)
        delete servers[i];
}


std::vector<std::pair<unsigned int, unsigned int> >
ServerCluster::getServersIPPortPairs() const
{
    std::vector<std::pair<unsigned int, unsigned int> >  pairs;
    std::pair<unsigned int, unsigned int>                pair;

    for (unsigned int i = 0; i < servers.size(); ++i)
    {
        Server& server = *servers[i];

        pair.first = server.getIP();

        const std::vector<unsigned int>& ports = server.getPorts();
        for (unsigned int j = 0; j < ports.size(); ++j)
        {
            pair.second = ports[j];
            pairs.push_back(pair);
        }
    }
    return (pairs);
}

bool    ServerCluster::isServerMatched(const Server& server, unsigned int ip, unsigned int port)
{
    const std::vector<unsigned int>& ports = server.getPorts();

    for (unsigned int i = 0; i < ports.size(); ++i)
    {
        if (ports.at(i) == port)
            if (server.getIP() == ip)
                return true;
    }
    return false;
}

Server*	ServerCluster::getMatchedServer(const IRequest &req)
{
    std::vector<Server *>           matchedServers;

    for (unsigned int i = 0; i < servers.size(); ++i)
        if (isServerMatched(*servers[i], req.getIncomingIP(), req.getIncomingPort()))
            matchedServers.push_back(servers[i]);

    if (matchedServers.size() > 1)
    {
        string reqHost = req.getHeader("host");
        int pos = reqHost.rfind(":");
        reqHost = reqHost.substr(0, pos);

        for (unsigned int i = 0; i < matchedServers.size(); ++i)
        {
            Server* server = matchedServers[i];
            const std::vector<string>&   names = server->getNames();

            if (std::find(names.begin(), names.end(), reqHost) != names.end())
                return (server);
        }
        
        for (unsigned int i = 0; i < matchedServers.size(); ++i)
            if (matchedServers[i]->isDefault())
                return (matchedServers[i]);

    }

    return (matchedServers.front());
}

Result  ServerCluster::handle(IRequest& request)
{
    Server* server = getMatchedServer(request);
    // std::cout << "reqest: " 
    //     << utils::ip(ntohl(request.getIncomingIP())) << " "
    //     << request.getIncomingPort() << std::endl;

    // std::cout << "match : " 
    //     << utils::ip(ntohl(server->getIP())) << " "
    //     << server->getPorts() << std::endl;
    return (server->handle(request));
}

