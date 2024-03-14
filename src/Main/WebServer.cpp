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
    std::vector<Config*> clusters = mConfig->getBlockConfigIfExist("cluster");
    if (clusters.size() != 1)
        throw ConfigException("Webserver: Invalid Config No Cluster Found ", "cluster", "Empty/Multiple");

    Config* clusterConfig = clusters.front();
    if ( ! clusterConfig->hasBlock("server") )
        throw ConfigException("Webserver: No Server Found", "server", "Empty");
    
    if (clusterConfig->hasInline("default_mime"))
        MimeTypes::setDefault(clusterConfig->getInlineConfig("default_mime"));

    std::vector<Config*>    mime_types = clusterConfig->getBlockConfig("mime_types");
    if (mime_types.size() == 1)
        MimeTypes::setMimeTypes(mime_types.front());
    else if (mime_types.size() == 0)
        MimeTypes::setMimeTypes(NULL);
    else
        throw ConfigException("webserve: Config error", "mime_type", "multiple definitions");

    mMux = new SelectMultiplexer();
    mServers = new ServerCluster( clusterConfig );

    std::vector<std::pair<unsigned int, unsigned int> > ip_port_pairs = mServers->getServersIPPortPairs();
    for (unsigned int i = 0; i < ip_port_pairs.size(); ++i)
    {
        unsigned int ip = ip_port_pairs[i].first;
        unsigned int port = ip_port_pairs[i].second;
        mSockets.push_back(ServerSocket(ip, port));
    }
}


WebServer::~WebServer()
{
    delete mMux;
    delete mServers;
}

void WebServer::start()
{
    for (std::vector<ServerSocket>::iterator it = mSockets.begin(); it != mSockets.end(); ++it)
    {
        ServerSocket& socket = *it;
        try {

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
        mMux->wait(1 * 1000000); // 1 seconds

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
        // function that loops and close connections to clients that need the connection to be closed (ex: timedout)
        // check timed out cgi scripts
        cleanup();
    }
}

void WebServer::acceptNewClients()
{
    std::queue<IServerSocket *> qs = mMux->getReadyServerSockets();
    ;
    while (qs.size())
    {
        const IServerSocket &sock = *(qs.front());

        IClientSocket *clientSock = sock.accept();

        clientSock->setNonBlocking();

        Client *client = new Client(clientSock, sock.getIP(), sock.getPort());

        mClients.push_back(client);

        mMux->add(client);

        qs.pop();
    }
}

void    WebServer::handleClientRequest(Client* client, IRequest* request)
{
    Request*    req = static_cast<Request*>(request);
    req->dump();

    mMux->remove(client);

    Result result = mServers->handle(*request);

    if (result.type == Result::RESPONSE)
    {
        Logger::debug ("Result::Response").flush();
        AResponse *response = static_cast<AResponse*>(result.response());
        client->setResponseHeaders(response);
        response->build();
        mMux->add(response);
        mResponses.push_back(response);

        response->client = client;
        client->status = Client::RECEIVING;
        client->activeResponse = response;

        delete request;
    }
    else if (result.type == Result::PROXY_PAIR)
    {
        Logger::debug ("Result::ProxyPair").flush();
        ProxyPair pair = result.proxyPair();

        client->status = Client::EXCHANGING;
        client->activeProxyPair = pair;

        // if (pair.request == NULL || pair.response == NULL) delete make 500 response

        if (pair.request->getSocketFd() != -1 && request->getMethod() != "GET") {
            mMux->add(pair.request, IMultiplexer::READ);
        }
        else {
            mMux->add(client);
        }

        mMux->add(pair.request, IMultiplexer::WRITE);

        mMux->add(pair.response, IMultiplexer::READ);
        mMux->add(pair.response, IMultiplexer::WRITE);
    }
    else if (result.type == Result::UPLOAD)
    {
        Logger::debug ("Result::Upload").flush();
        Upload* upload = result.upload();

        upload->client = client;
        client->activeUpload = upload;
        client->status = Client::RECEIVING;

        mMux->add(upload);
    }
}

void WebServer::takeAndHandleRequests()
{
    std::queue<IClient *> qc = mMux->getReadyClients();
    while (qc.size())
    {
        Client *client = static_cast<Client *>(qc.front());

        client->makeRequest();
        if (client->status == Client::DISCONNECTED)
        {
            disconnectClient(*client);
            delete client;
        }
        else if (client->hasRequest())
            handleClientRequest(client, client->getRequest()); 
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
            Client* client = static_cast<Client*>(res->client);
            client->activeResponse = NULL;
            client->status = Client::IDLE;
            mMux->remove(res);
            mMux->add(client);
            delete res;
        }
        qr.pop();
    }
}

void  WebServer::handleUploads()
{
    std::queue<IUpload *> qu = mMux->getReadyUploads();
    while (qu.size())
    {
        Upload *upload = static_cast<Upload*>(qu.front());
        try
        {
            upload->handle();
            if (upload->done())
            {
                Logger::debug ("upload done").flush();
                Client* client = static_cast<Client*>(upload->client);
                mMux->remove(upload);
                // mMux->add(client);
                handleClientRequest(client, upload->getRequest());
                Logger::debug ("upload request handled").flush();
                /*
                    IRequest *request = upload->getRequest();

                    AResponse *response = new BufferResponse(request->getSocket());
                    response->setStatusCode(201)
                        .setHeader("Content-Type", "text/plain")
                        .setBody("upload")
                        .build();
                    
                    delete request;
                    */
                    /* mMux->add(response);

                    client->activeUpload = NULL;
                    client->activeResponse = response;
                    client->status = Client::RECEIVING;
                    response->client = client;
                */
                delete upload;
            }
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
        }
        
        qu.pop();
    }
}

void WebServer::readFromReadyProxyRequests()
{
    std::queue<IProxyRequest *> qpreqs = mMux->getReadyForReadingProxyRequests();

    while (qpreqs.size())
    {
        IProxyRequest *req = qpreqs.front();

        try
        {
            req->read();
            // if (req->done())
            // {
            //     mMux->remove(req, IMultiplexer::WRITE);
            //     if (req->getSocketFd() != -1)
            //     {
            //         mMux->remove(req, IMultiplexer::READ);

            //         std::vector<Client *>::iterator itc = mClients.begin();
            //         while (itc != mClients.end())
            //         {
            //             Client &client = **itc;
            //             if (client.activeProxyPair.request == req)
            //             {
            //                 client.activeProxyPair.request = NULL;
            //                 mMux->add(&client);
            //                 break;
            //             }
            //             ++itc;
            //         }
            //     }
            //     delete req;
            // }
        }
        catch (const RequestException &e)
        {
            Logger::debug ("cgi request error ")( e.what() ).flush();
            if (e.error == RequestException::CONNECTION_CLOSED)
            {
                std::vector<Client *>::iterator itc = mClients.begin();
                while (itc != mClients.end())
                {
                    Client &client = **itc;
                    if (client.activeProxyPair.request == req)
                    {
                        disconnectClient(client);
                        break;
                    }
                    ++itc;
                }
            }
            else
            {
                // bad request;
            }
        }

        qpreqs.pop();
    }
}

void WebServer::sendReadyProxyRequests()
{
    std::queue<IProxyRequest *> qpreqs = mMux->getReadyForWritingProxyRequests();

    while (qpreqs.size())
    {
        IProxyRequest *req = qpreqs.front();

        try
        {
            req->send();
            if (req->done())
            {
                mMux->remove(req, IMultiplexer::WRITE);
                if (req->getSocketFd() != -1)
                    mMux->remove(req, IMultiplexer::READ);
                // {

                    std::vector<Client *>::iterator itc = mClients.begin();
                    while (itc != mClients.end())
                    {
                        Client &client = **itc;
                        if (client.activeProxyPair.request == req)
                        {
                            client.activeProxyPair.request = NULL;
                            mMux->add(&client);
                            break;
                        }
                        ++itc;
                    }
                // }
                delete req;
            }
        }
        catch (const std::exception &e)
        {
            Logger::debug ("cgi sending error ")( e.what() ).flush();

            std::vector<Client *>::iterator itc = mClients.begin();
            while (itc != mClients.end())
            {
                Client &client = **itc;
                if (client.activeProxyPair.request == req)
                    break;
                ++itc;
            }
            Client &client = **itc;

            if (client.activeProxyPair.request->getSocketFd() != -1)
                mMux->remove(client.activeProxyPair.request, IMultiplexer::READ);
            mMux->remove(client.activeProxyPair.request, IMultiplexer::WRITE);
            delete client.activeProxyPair.request;
            client.activeProxyPair.request = NULL;

            mMux->remove(client.activeProxyPair.response, IMultiplexer::READ);
            mMux->remove(client.activeProxyPair.response, IMultiplexer::WRITE);
            client.activeProxyPair.setChildFree();

            if (!client.activeProxyPair.response->sent()) // if nothing is sent to the client
            {
                BufferResponse *res = new BufferResponse(client.getSocket());
                res->client = &client; 

                std::string body = "<center>500</center><br><center>server error</center>";
                res->setStatusCode(500).setHeader("Content-type", "text/html").setBody(body).build();

                mMux->add(res);
                mResponses.push_back(res);

                client.status = Client::RECEIVING;
                client.activeResponse = res;
            }
            delete client.activeProxyPair.response;
            client.activeProxyPair.response = NULL;

            // clean
            // if cgi program exited generate an error page if possible

            // cleaning:
            // request
            // generate appropriet response if possible
        }

        qpreqs.pop();
    }
}

void WebServer::readFromReadyProxyResponses()
{
    // cgi progrpams which exited could be detected here
    // need to be handled here
    // clening:
    // request need to be removed
    // generate an error response if possible
    std::queue<IProxyResponse *> qpres = mMux->getReadyForReadingProxyResponses();
    while (qpres.size())
    {
        IProxyResponse *res = qpres.front();
        try
        {
            Logger::debug ("cgi response reading").flush();
            res->read();
            Logger::debug ("cgi response reading done.").flush();
        }
        catch (const std::exception &e)
        {
            Logger::warn ("cgi response error ")( e.what() ).flush();
        }

        qpres.pop();
    }
    // read;
    // if return is -1
    // if already sent header
    // remove proxypair from webserver loop
    // else
    // build error Response
    // check end of header (\r\n\r\n or \n\n)
    // build response
}

void WebServer::sendReadyProxyResponses()
{
    // clients who closed connections could be detected here
    // need to be handled here
    // cleaning:
    // client
    // request
    // response
    std::queue<IProxyResponse *> qpres = mMux->getReadyForWritingProxyResponses();
    
    while (qpres.size())
    {
        IProxyResponse *res = qpres.front();

        try
        {
            res->send();
            if (res->done() || res->error())
            {
                mMux->remove(res, IMultiplexer::READ);
                mMux->remove(res, IMultiplexer::WRITE);
                std::vector<Client *>::iterator itc = mClients.begin();
                while (itc != mClients.end())
                {
                    Client &client = **itc;
                    if (client.getSocketFd() == res->getSocketFd())
                    {
                        Logger::debug ("response setting child free").flush();
                        client.activeProxyPair.setChildFree();
                        client.activeProxyPair.response = NULL;
                        client.status = Client::IDLE;
                        mMux->add(&client);
                        break;
                    }
                    ++itc;
                }
                if (res->error())
                {
                    Client &client = **itc;
                    BufferResponse *res = new BufferResponse(client.getSocket());
                    res->client = &client;

                    std::string body = "500<br><center>server error</center>";
                    res->setStatusCode(500).setHeader("Content-type", "text/html").setBody(body).build();

                    mMux->add(res);
                    mResponses.push_back(res);

                    client.status = Client::RECEIVING;
                    client.activeResponse = res;
                }
                delete res;
                // Client& client = **itc;
                // client.resetTimeout();
            }
        }
        catch (const std::exception &e)
        {
            Logger::warn ("cgi response error ")( e.what() ).flush();
        }

        qpres.pop();
    }
    // if built and buffer not empty
    // send from buffer
    // else
    // return;
}

void WebServer::disconnectClient(Client &client)
{
    if (client.activeResponse)
    {
        mMux->remove(client.activeResponse);
        std::vector<IResponse *>::iterator it = std::find(mResponses.begin(), mResponses.end(), client.activeResponse);
        mResponses.erase(it);
        delete client.activeResponse;
    }

    if (client.activeProxyPair.request)
    {
        mMux->remove(client.activeProxyPair.request, IMultiplexer::READ);
        mMux->remove(client.activeProxyPair.request, IMultiplexer::WRITE);
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

        if (client->hasTimedOut() || ! client->isKeptAlive()) {
            vector<Client*>::iterator tmp = it - 1;
            // mClients.erase(it);
            disconnectClient(*client);
            delete client;
            it = tmp;
        }
        ++it;
    }
}
