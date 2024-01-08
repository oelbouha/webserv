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

ServerCluster::ServerCluster()
{}

ServerCluster::ServerCluster( const ServerCluster& s )
{(void)s;}

ServerCluster::~ServerCluster()
{}

ServerCluster&	ServerCluster::operator=( const ServerCluster& s )
{
    (void)s;
	return (*this);
}

IResponse*  ServerCluster::handle(IRequest* request)
{
    (void)request;
    std::string file = "pages/index.html";
    std::string mime = "text/html";

    if (request->getURI() != "/"){
        file = "pages/";
        const std::string&    uri = request->getURI();
        file += uri.substr(uri.rfind('/'));
        if (file.rfind('.') == std::string::npos)
            mime = "text/plain";
        else {
            std::string extension = file.substr(file.rfind('.') + 1);
            std::cout << "extension: " << extension << std::endl;
            if (extension == "html")
                mime = "text/html";
            else if (extension == "jpeg")
                mime = "image/jpeg";
            else if (extension == "mp4")
                mime = "video/mp4";
        }
    }

    Response*   response = new Response(request->getSocket());

    response->setStatusCode(200)
        .setHeader("content-type", mime)
        .setHeader("connection", "keep-alive")
        .setBody("hello from webserve")
        .setBodyFile(file)
        .build();

    return (response);
}

/*
#include "src/Server/CGIHandler.hpp"

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
