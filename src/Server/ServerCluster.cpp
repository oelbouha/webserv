/*	                     __          _
 *	   __  ___________ _/ /___ ___  (_)
 *	  / / / / ___/ __ `/ / __ `__ \/ /
 *	 / /_/ (__  ) /_/ / / / / / / / / 
 *	 \__, /____/\__,_/_/_/ /_/ /_/_/ 
 *	/____/	User: Youssef Salmi
 *			File: ServerCluster.cpp 
 */

#include "ServerCluster.hpp"
#include <cstring>


ServerCluster::ServerCluster() {}

ServerCluster::ServerCluster( const ServerCluster& s ) {(void)s;}

ServerCluster&	ServerCluster::operator=( const ServerCluster& s ) {
    (void)s;
	return (*this);
}

// IResponse*  ServerCluster::handle(IRequest* request)
// {
//     std::string file = "pages/index.html";
//     std::string mime = "text/html";

//     std::cout << "req.uri: " << request->getURI() << std::endl << std::flush;

//     if (request->getURI() != "/"){
//         file = "pages";
//         const std::string&    uri = request->getURI();
//         file += uri.substr(uri.rfind('/'));
//         if (file.rfind('.') == std::string::npos)
//             mime = "text/plain";
//         else {
//             std::string extension = file.substr(file.rfind('.') + 1);
//             if (extension == "html")
//                 mime = "text/html";
//             else if (extension == "jpeg")
//                 mime = "image/jpeg";
//             else if (extension == "mp4")
//                 mime = "video/mp4";
//             else if (extension == "ico")
//                 mime = "image/png";
//         }
//             response->setStatusCode(200)
//         .setHeader("content-type", mime)
//         .setHeader("connection", "keep-alive")
//         // .setBody("hello from webserve")
//         .setBodyFile(file)
//         .build();
//     }
// }


ServerCluster::~ServerCluster() {
    delete error_pages;
    for(size_t i = 0; i < servers.size(); ++i)
        delete servers[i];
}

ServerCluster::ServerCluster(Config* config) : UriMaxlength(2048), server(NULL) {
    error_pages = new ErrorPage();

    Config* cluster = config->getBlockConfig("cluster").front();
    
    error_pages->setErrorPage(*cluster);
    std::string alive = cluster->getInlineConfigIfExist("keep_alive");
    KeepAlive = std::stod(alive, NULL);

    std::vector<Config *> ServersConfig = cluster->getBlockConfigIfExist("server");
    std::vector<Config *>::iterator it = ServersConfig.begin();
    
    while (it != ServersConfig.end()){
        Config  *serverConfig = *it;

        serverConfig->addBlockIfExist(*cluster, "error_page");
		serverConfig->addListIfExist(*cluster, "cgi");

        Server *server = new Server(serverConfig, *error_pages);
        servers.push_back(server);
        ++it;
    }
}

ErrorPage& 	ServerCluster::getErrorPage() const { return *error_pages; }

unsigned int ServerCluster::getStatusCode() const  { return statusCode; }


string  ServerCluster::getRoot() const { return server->getRoot(); }

bool	ServerCluster::containsValidCharacters(string uri) {
    const string& validCharacters = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghij\
        klmnopqrstuvwxyz0123456789-._~:/?#[]@!$&'()*+,;=%";
    for (size_t i = 0; i < uri.length(); ++i){
        if (validCharacters.find(uri[i]) == std::string::npos)
            return false;
    }
	return true;
}


#include "src/CGI/CGIHandler.hpp"

ProxyPair   ServerCluster::handleCGI(IRequest* request)
{
    CGIHandler    handler;

    return (handler.handle(request));
/*    std::string file = "pages/index.py";
    response.*/
}

bool	ServerCluster::isRequestProperlyStructured(const IRequest &req) {
    // try {
    //     const string& transfer  = req.getHeader("transfer-encoding");
    //     if (transfer != "chunked") {
    //         std::cout << "request not well formed\n" << std::endl;
    //         statusCode = 501;
    //         return false;
    //     }
    // }
    // catch(...) {}

    if (containsValidCharacters(req.getURI()) == false) {
        statusCode = 400;
        return false;
    }
    else if (req.getURI().length() > UriMaxlength) {
        statusCode = 414;
        return false;
    }
    return true;
}

bool	ServerCluster::isServerMatched(const Server& server, const IRequest& req) {
    const string& serverHost = server.getHost();
    std::vector<string> ports = server.getPort();

    unsigned int inComingPort = req.getIncomingPort();
    const string& inComingHost = utils::UintToIp(req.getIncomingIP());

    // std::cout << "incoming ip  ------> " << inComingHost << std::endl;
    // std::cout << "incoming port -----> " << inComingPort << std::endl;

    std::vector<string>::iterator it = ports.begin();
    while(it != ports.end()) {
        unsigned int serverPort = std::stod(*it, NULL);
        if (serverPort == inComingPort) {
            if (serverHost == inComingHost)
                return true;
        }
        ++it;
    }
    return false;
}

Server*	ServerCluster::getDefaultServer() {
    std::vector<Server *>::iterator it = servers.begin();
    while (it != servers.end()){
        Server *server = *it;
        if (server->isDefault())
            return (server);
        ++it;
    }
    return (servers[0]);
}


Server*	ServerCluster::getMatchedServer(const IRequest &req) {
    std::vector<Server *> MatchedServers;
    std::vector<Server *>::iterator it = servers.begin();

    while (it != servers.end()) {
        Server *server = *it;
        if (isServerMatched(*server, req))
            MatchedServers.push_back(server);
        ++it;
    }

    string reqHost = req.getHeader("Host");
    int pos = reqHost.rfind(":");
    reqHost = reqHost.substr(0, pos);

    it = MatchedServers.begin();
    while (it != MatchedServers.end()) {
        Server *server = *it;
        std::vector<string> names = server->getName();
        std::vector<string>::iterator find = std::find(names.begin(), names.end(), reqHost);
        if (find != names.end())
            return (server);
        ++it;
    }
    return (getDefaultServer());
}


IResponse*  ServerCluster::handle(IRequest& request) {
    server = getMatchedServer(request);
    if (isRequestProperlyStructured(request) == false) {
        std::cout << " Request not properly structered ... " << statusCode << std::endl;;
        return (Helper::BuildResponse(request, *this));
    }
    IResponse * res = server->handle(request);
    return (res);
}

