/*	                     __          _
 *	   __  ___________ _/ /___ ___  (_)
 *	  / / / / ___/ __ `/ / __ `__ \/ /
 *	 / /_/ (__  ) /_/ / / / / / / / /
 *	 \__, /____/\__,_/_/_/ /_/ /_/_/
 *	/____/	User: Youssef Salmi
 *			File: WebServer.cpp
 */

#include "WebServer.hpp"
#include <algorithm>
#include <queue>
#include <vector>

WebServer::WebServer(Config *aConfig) : mConfig(aConfig)
{
    // Cluster
    std::vector<Config*> clusters = mConfig->getBlockConfigIfExist("cluster");
    if (clusters.size() != 1)
        throw ConfigException("Invalid Config No Cluster Found ", "cluster", "Empty/Multiple");

    Config* clusterConfig = clusters.front();
    if ( ! clusterConfig->hasBlock("server") )
        throw ConfigException("No Server Found", "server", "Empty");
    
    // Mime types
    if (clusterConfig->hasInline("default_mime"))
        MimeTypes::setDefault(clusterConfig->getInlineConfig("default_mime"));

    std::vector<Config*>    mime_types = clusterConfig->getBlockConfig("mime_types");
    if (mime_types.size() == 1)
        MimeTypes::setMimeTypes(mime_types.front());
    else if (mime_types.size() == 0)
        MimeTypes::setMimeTypes(NULL);
    else
        throw ConfigException("Config error", "mime_type", "multiple definitions");
    
    // Keep alive
    try {
        int keep_alive_limit = 50;
        int keep_alive_timeout = 10;
        if (clusterConfig->hasInline("keep_alive"))
            keep_alive_limit = utils::string_to_int(clusterConfig->getInlineConfig("keep_alive"));
        if (clusterConfig->hasInline("keep_alive_timeout"))
            keep_alive_timeout = utils::string_to_int(clusterConfig->getInlineConfig("keep_alive_timeout"));
        if (keep_alive_timeout == 0) keep_alive_limit = 0;

        Client::setKeepAlive(keep_alive_limit, keep_alive_timeout);
    }
    catch (const std::exception& e)
    { throw ConfigException("Config error", "keep_alive/keep_alive_timeout", e.what()); }


    mMux = new SelectMultiplexer();
    mCluster = new Cluster( clusterConfig );

    std::set<std::pair<uint, uint> > ip_port_pairs = mCluster->getServersIPPortPairs();

    std::set<std::pair<uint, uint> >::iterator  it = ip_port_pairs.begin();
    for (; it != ip_port_pairs.end(); ++it)
    {
        unsigned int ip = it->first;
        unsigned int port = it->second;
        mSockets.push_back(ServerSocket(ip, port));
    }
}

WebServer::~WebServer()
{
    delete mMux;
    delete mCluster;
}


void WebServer::start()
{
    for (std::vector<ServerSocket>::iterator it = mSockets.begin(); it != mSockets.end(); ++it)
    {
        ServerSocket& socket = *it;
        try {
            socket.setNonBlocking();
            socket.bind();
            socket.listen();
            mMux->add(it.base());

            Logger::info ("Listening on ")( utils::ip(ntohl(socket.getIP())) );
            Logger::info (" on port ")(socket.getPort()).flush();
        }
        catch (const SocketException& e) {
            std::vector<ServerSocket>::iterator it_tmp = it--;
            it_tmp->close();
            mSockets.erase(it_tmp);
            Logger::warn ( e.what() ).flush();
        }
    }

    if (mSockets.empty()) {
        Logger::fatal ("No active listening Socket").flush();
        std::exit(EXIT_FAILURE);
    }
}

void WebServer::loop()
{
    while (true)
    {
        mMux->wait(1 * 1000000);
        if (mMux->ready())
        {
            acceptNewClients();
            takeAndHandleRequests();
            sendResponses();
            readFromReadyProxyRequests();
            sendReadyProxyRequests();
            readFromReadyProxyResponses();
            sendReadyProxyResponses();
            handleUploads();
        }
        cleanup();
    }
}

void WebServer::acceptNewClients()
{
    std::queue<IServerSocket *> qs = mMux->getReadyServerSockets();
    while (qs.size())
    {
        const IServerSocket&    sock = *(qs.front());

        IClientSocket*  clientSock = sock.accept();
        Client*         client = new Client(clientSock, sock.getIP(), sock.getPort());

        clientSock->setNonBlocking();
        mClients.push_back(client);
        mMux->add(client);
        qs.pop();
    }
}

void WebServer::handleClientRequest(Client* client, Request* request)
{
    Result result = mCluster->handle(*request);

    mMux->remove(client);

    client->setKeepAlive(request->getHeader("connection"));

    if (result.type == Result::RESPONSE)
    {
        AResponse* response = static_cast<AResponse*>(result.response());
        client->setResponseHeaders(response);
        response->build();
        mMux->add(response);

        response->client = client;
        client->status = Client::ACTIVE;
        client->activeResponse = response;

        delete request;
    }
    else if (result.type == Result::PROXY_PAIR)
    {
        ProxyPair       pair = result.proxyPair();
        CGIRequest*     cgi_request = static_cast<CGIRequest*>(pair.request);
        CGIResponse*    cgi_response = static_cast<CGIResponse*>(pair.response);

        try { cgi_request->read(); }
        catch ( const RequestException& e )
        {
            pair.setChildFree();
            AResponse* response = static_cast<AResponse*>(cgi_request->buildErrorPage(e.error));
            client->setResponseHeaders(response);
            response->build();
            mMux->add(response);

            response->client = client;
            client->status = Client::ACTIVE;
            client->activeResponse = response;
            client->activeProxyPair = ProxyPair();
            delete cgi_request;
            delete cgi_response;
            return ;
        }
        catch (const SocketException& e)
        { disconnectClient(*client); delete client; return ;}

        client->setResponseHeaders(cgi_response);

        client->status = Client::ACTIVE;
        client->activeProxyPair = pair;
        cgi_request->client = client;
        cgi_response->client = client;

        mMux->add(cgi_response, IMultiplexer::READ);
        mMux->add(pair.request, IMultiplexer::WRITE);
        if (pair.request->getSocketFd() != -1 && request->getMethod() != "GET")
            mMux->add(pair.request, IMultiplexer::READ);
        else
            mMux->add(client);
    }
    else if (result.type == Result::UPLOAD)
    {
        Upload* upload = result.upload();

        if (upload->done())
        {
            handleClientRequest(client, upload->getRequest());
            delete upload;
        }
        else
        {
            mMux->add(upload);
            client->activeUpload = upload;
            client->status = Client::ACTIVE;
            upload->client = client;
        }
    }
}

void WebServer::takeAndHandleRequests()
{
    std::queue<IClient *> qc = mMux->getReadyClients();
    while (qc.size())
    {
        Client *client = static_cast<Client *>(qc.front());
        try {
            client->makeRequest();
            if (client->hasRequest())
                handleClientRequest(client, client->getRequest()); 
        }
        catch (const SocketException&)
        { disconnectClient(*client); delete client; }
        qc.pop();
    }
}

void WebServer::sendResponses()
{
    std::queue<IResponse *> qr = mMux->getReadyResponses();
    while (qr.size())
    {
        AResponse *res = static_cast<AResponse*>(qr.front());

        res->send();
        if (res->done())
        {
            Client& client = *static_cast<Client*>(res->client);
            client.activeResponse = NULL;
            client.status = Client::IDLE;
            client.resetTimeout();
            mMux->remove(res);
            mMux->add(&client);
            delete res;
        }
        qr.pop();
    }
}

void WebServer::handleUploads()
{
    std::queue<IUpload *> qu = mMux->getReadyUploads();

    while (qu.size())
    {
        Upload *upload = static_cast<Upload*>(qu.front());
        Client* client = static_cast<Client*>(upload->client);
        try
        {
            upload->handle();
            if (upload->done())
            {
                mMux->remove(upload);
                handleClientRequest(client, upload->getRequest());
                delete upload;
            }
        }
        catch (const SocketException&)
        { disconnectClient(*client); delete client; }
        catch (const RequestException& e)
        {
            AResponse *response = static_cast<AResponse*>(upload->buildErrorPage(e.error));

            response->setHeader("Connection", "close").build();
            client->setKeepAlive("close");

            mMux->remove(upload);
            mMux->add(response);

            response->client = client;
            client->status = Client::ACTIVE;
            client->activeResponse = response;
            client->activeUpload = NULL;

            delete upload;
        }
        catch(const std::exception& e)
        {
            mMux->remove(upload);
            delete upload;
            Logger::error (e.what() ).flush();
        }
        
        qu.pop();
    }
}

void WebServer::readFromReadyProxyRequests()
{
    std::queue<IProxyRequest *> qpreqs = mMux->getReadyForReadingProxyRequests();

    while (qpreqs.size())
    {
        CGIRequest* req = static_cast<CGIRequest*>(qpreqs.front());
        Client*     client = static_cast<Client*>(req->client);

        try { req->read(); }
        catch (const RequestException &e)
        {
            mMux->remove(req, IMultiplexer::WRITE);
            if (req->getSocketFd() != -1) mMux->remove(req, IMultiplexer::READ);

            AResponse *response = static_cast<AResponse*>(req->buildErrorPage(e.error));

            response->setHeader("Connection", "close");
            response->build();
            client->setKeepAlive("close");

            mMux->add(response);

            response->client = client;
            client->status = Client::ACTIVE;
            client->activeResponse = response;

            
            client->activeProxyPair.setChildFree();
            delete client->activeProxyPair.request;
            delete client->activeProxyPair.response;
            client->activeProxyPair.request = NULL;
            client->activeProxyPair.response = NULL;
        }
        catch (const SocketException& e)
        { disconnectClient(*client); delete client; }

        qpreqs.pop();
    }
}

void WebServer::sendReadyProxyRequests()
{
    std::queue<IProxyRequest *> qpreqs = mMux->getReadyForWritingProxyRequests();

    while (qpreqs.size())
    {
        CGIRequest *req = static_cast<CGIRequest*>(qpreqs.front());

        try
        {
            Logger::debug ("cgi request send").flush();
            req->send();
            if (req->done())
            {
                Logger::debug ("cgi req done").flush();
                mMux->remove(req, IMultiplexer::WRITE);
                if (req->getSocketFd() != -1)
                    mMux->remove(req, IMultiplexer::READ);
                Client &client = *static_cast<Client*>(req->client);
                client.activeProxyPair.request = NULL;
                mMux->add(&client);
                delete req;

                mMux->add(client.activeProxyPair.response, IMultiplexer::READ);
                mMux->add(client.activeProxyPair.response, IMultiplexer::WRITE);
            }
        }
        catch (const std::exception &e)
        {
            Logger::debug ("cgi ACTIVE error ")( e.what() ).flush();

            Client &client = *static_cast<Client*>(req->client);

            if (client.activeProxyPair.request->getSocketFd() != -1)
                mMux->remove(client.activeProxyPair.request, IMultiplexer::READ);
            mMux->remove(client.activeProxyPair.request, IMultiplexer::WRITE);
            client.activeProxyPair.setChildFree();

            if (!client.activeProxyPair.response->sent()) // if nothing is sent to the client
            {
                AResponse *res = static_cast<AResponse*>(req->buildErrorPage(502));
                res->client = &client;
                
                client.setResponseHeaders(res);
                res->build();
                mMux->add(res);

                client.status = Client::ACTIVE;
                client.activeResponse = res;
            }

            delete client.activeProxyPair.request;
            client.activeProxyPair.request = NULL;
            delete client.activeProxyPair.response;
            client.activeProxyPair.response = NULL;
        }

        qpreqs.pop();
    }
}

void WebServer::readFromReadyProxyResponses()
{
    std::queue<IProxyResponse *> qpres = mMux->getReadyForReadingProxyResponses();
    while (qpres.size())
    {
        CGIResponse *res = static_cast<CGIResponse*>(qpres.front());
        Client*     client = static_cast<Client*>(res->client);
        try
        {
            res->read();
            if (res->isLocalRedirection()) {
                Request*    req = static_cast<Request*>(res->request);
                const std::string&  location = res->getRedirectionLocation();
                std::string         raw_req_header = req->getRawHeader();
                raw_req_header.erase(0, raw_req_header.find("\r\n") + 2);
                raw_req_header = "GET " + location + " HTTP/1.1\r\n" + raw_req_header;
                
                BufferRequest* redir_request = new BufferRequest(*req, raw_req_header, "");

                mMux->remove(res, IMultiplexer::READ);
                mMux->remove(res, IMultiplexer::WRITE);

                client->activeProxyPair.setChildFree();
                client->activeProxyPair.response = NULL;

                handleClientRequest(client, redir_request);

                delete res->request;
                delete res;
            }
        }
        catch (const std::exception &e)
        {
            Logger::warn ("cgi response error ")( e.what() ).flush();
        }

        qpres.pop();
    }
}

void WebServer::sendReadyProxyResponses()
{
    std::queue<IProxyResponse *> qpres = mMux->getReadyForWritingProxyResponses();
    
    while (qpres.size())
    {
        CGIResponse*    res = static_cast<CGIResponse*>(qpres.front());
        Client*         client = static_cast<Client*>(res->client);
        try
        {
            res->send();
            if (res->done() || res->error())
            {
                mMux->remove(res, IMultiplexer::READ);
                mMux->remove(res, IMultiplexer::WRITE);

                client->activeProxyPair.setChildFree();
                client->activeProxyPair.response = NULL;
                
                if (res->error())
                {
                    AResponse* response = static_cast<AResponse*>(res->buildErrorPage(502));

                    response->client = client;
                    client->setResponseHeaders(response);
                    response->build();

                    mMux->add(response);

                    client->status = Client::ACTIVE;
                    client->activeResponse = response;
                }
                else
                {
                    mMux->add(client);
                    client->status = Client::IDLE;
                    client->resetTimeout();
                }
                delete res->request;
                delete res;
            }
        }
        catch (const std::exception &e)
        {
            Logger::warn ("cgi response error ")( e.what() ).flush();
        }

        qpres.pop();
    }
}

void WebServer::disconnectClient(Client &client)
{
    if (client.activeResponse)
    {
        mMux->remove(client.activeResponse);
        delete client.activeResponse;
    }

    if (client.activeProxyPair.request)
    {
        mMux->remove(client.activeProxyPair.request, IMultiplexer::READ);
        mMux->remove(client.activeProxyPair.request, IMultiplexer::WRITE);
        client.activeProxyPair.setChildFree();
        delete client.activeProxyPair.request;
    }

    if (client.activeProxyPair.response)
    {
        mMux->remove(client.activeProxyPair.response, IMultiplexer::READ);
        mMux->remove(client.activeProxyPair.response, IMultiplexer::WRITE);
        client.activeProxyPair.setChildFree();
        delete client.activeProxyPair.response;
    }

    mMux->remove(&client);

    std::vector<Client *>::iterator it = std::find(mClients.begin(), mClients.end(), &client);
    mClients.erase(it);
}

void WebServer::cleanup()
{
    vector<Client*>::iterator it = mClients.begin();

    while (it != mClients.end()) {
        Client* client = *it;

        if (client->hasTimedOut()) {
            vector<Client*>::iterator tmp = it - 1;
            disconnectClient(*client);
            delete client;
            it = tmp;
        }
        if (client->activeProxyPair.response) {
            CGIResponse* response = static_cast<CGIResponse*>(client->activeProxyPair.response);
            IRequest*    request = response->request;
            if (response->hasTimedOut()) {
                if (response->sent() > 0) {
                    Logger::warn ("Cgi response timed out:")(request->getMethod())(" request to ");
                    Logger::warn (request->getURI()).flush();
                    vector<Client*>::iterator tmp = it - 1;
                    disconnectClient(*client);
                    delete client;
                    it = tmp;
                }
                else {
                    mMux->remove(response, IMultiplexer::READ);
                    mMux->remove(response, IMultiplexer::WRITE);

                    client->activeProxyPair.setChildFree();
                    client->activeProxyPair.response = NULL;

                    AResponse* res = static_cast<AResponse*>(response->buildErrorPage(504));

                    res->client = client;
                    client->setResponseHeaders(res);
                    res->build();

                    mMux->add(res);

                    client->status = Client::ACTIVE;
                    client->activeResponse = res;
                    delete response;
                    delete request;
                }
            }
        }
        ++it;
    }
}
