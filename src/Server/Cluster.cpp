/*	                     __          _
 *	   __  ___________ _/ /___ ___  (_)
 *	  / / / / ___/ __ `/ / __ `__ \/ /
 *	 / /_/ (__  ) /_/ / / / / / / / / 
 *	 \__, /____/\__,_/_/_/ /_/ /_/_/ 
 *	/____/	User: Youssef Salmi
 *			File: Cluster.cpp 
 */

#include "Cluster.hpp"


Cluster::Cluster( const Cluster& s ) {(void)s;}

Cluster&	Cluster::operator=( const Cluster& s )
{
    (void)s;
	return (*this);
}


Cluster::Cluster(Config* cluster)
{    
    std::string root = cluster->getInlineConfigOr("root", CLUSTER_DEFAULT_ROOT);
    error_pages.setErrorPages(cluster->getBlockConfigIfExist("error_page"), root);

    std::vector<Config *> ServersConfig = cluster->getBlockConfigIfExist("server");
    std::vector<Config *>::iterator it = ServersConfig.begin();

    while (it != ServersConfig.end())
    {
        Config&  serverConfig = **it;

		serverConfig.addListIfExist(*cluster, "cgi");
        serverConfig.addInlineIfNotExist(*cluster, "root");
        serverConfig.addInlineIfNotExist(*cluster, "max_body_size");

        servers.push_back(new Server(serverConfig, error_pages));
        ++it;
    }
}

Cluster::~Cluster()
{
    for(size_t i = 0; i < servers.size(); ++i)
        delete servers[i];
}


std::set<std::pair<unsigned int, unsigned int> >
Cluster::getServersIPPortPairs() const
{
    std::set<std::pair<unsigned int, unsigned int> >  pairs;
    std::pair<unsigned int, unsigned int>             pair;

    for (unsigned int i = 0; i < servers.size(); ++i)
    {
        Server& server = *servers[i];

        pair.first = server.getIP();

        const std::vector<unsigned int>& ports = server.getPorts();
        for (unsigned int j = 0; j < ports.size(); ++j)
        {
            pair.second = ports[j];
            pairs.insert(pair);
        }
    }
    return (pairs);
}

bool    Cluster::isServerMatched(const Server& server, unsigned int ip, unsigned int port)
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

Server*	Cluster::getMatchedServer(Request &req)
{
    std::vector<Server *>           matchedServers;

    for (unsigned int i = 0; i < servers.size(); ++i)
        if (isServerMatched(*servers[i], req.incomingIP, req.incomingPort))
            matchedServers.push_back(servers[i]);

    if (matchedServers.size() > 1)
    {
        req.build();
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

Result  Cluster::handle(Request& request)
{
    try {
        Server* server = getMatchedServer(request);
        return (server->handle(request));
    }
    catch ( const RequestException& e ) {
        return Result(error_pages.build(request, e.error));
    }
}

