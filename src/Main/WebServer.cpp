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
        try {

            it->bind();
            it->listen();
            mMux->add(*it);
            std::cout << "listening on " << std::right << std::setw(15) << utils::ip(ntohl(it->getIP())) << " on port " << it->getPort() << std::endl;

        } catch (const SocketException& e) {
            std::cerr << "webserv: " << e.what() << std::endl;
        }
    }
}

void WebServer::loop()
{
    while (true)
    {
        mMux->wait(1 * 1000000); // 1 seconds

        if (!mMux->ready())
        {
            std::cout << "." << std::flush;
            continue;
        }
        std::cout << std::endl;

        acceptNewClients();
        takeAndHandleRequests();
        sendResponses();
        readFromReadyProxyRequests();
        sendReadyProxyRequests();
        readFromReadyProxyResponses();
        sendReadyProxyResponses();
        handleUploads();
        // function that loops and close connections to clients that need the connection to be closed (ex: timedout)
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

        mMux->add(*client);

        qs.pop();
    }
    // std::cout << "Done.\n" << std::flush;
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
        {
            mMux->remove(*client);
            IRequest *request = client->getRequest();

            Result result = mServers->handle(*request);

            if (result.type == Result::RESPONSE)
            {
                IResponse *response = result.response();
                // client->setResponseHeaders(response);
                mMux->add(*response);
                mResponses.push_back(response);

                client->status = Client::RECEIVING;
                client->activeResponse = response;

                delete request;
            }
            else if (result.type == Result::PROXY_PAIR)
            {
                ProxyPair pair = result.proxyPair();

                client->status = Client::EXCHANGING;
                client->activeProxyPair = pair;

                // if (pair.request == NULL || pair.response == NULL) delete make 500 response
                if (pair.request->getSocketFd() != -1 && request->getMethod() != "GET")
                    mMux->add(pair.request, IMultiplexer::READ);
                else
                    mMux->add(*client);
                mMux->add(pair.request, IMultiplexer::WRITE);

                mMux->add(pair.response, IMultiplexer::READ);
                mMux->add(pair.response, IMultiplexer::WRITE);
            }
            else if (result.type == Result::UPLOAD)
            {
                Upload* upload = result.upload();

                mMux->add(upload);
                std::cout << "Upload add\n" << std::endl;

                client->status = Client::RECEIVING;
                client->activeUpload = upload;
            }
        }
        qc.pop();
    }
}

void WebServer::sendResponses()
{
    std::queue<IResponse *> qr = mMux->getReadyResponses();
    while (qr.size())
    {
        IResponse *res = qr.front();

        res->send();
        if (res->done())
        {
            mMux->remove(*res);
            std::vector<IResponse *>::iterator it = std::find(mResponses.begin(), mResponses.end(), res);
            mResponses.erase(it);

            std::vector<Client *>::iterator itc = mClients.begin();
            while (itc != mClients.end())
            {
                Client *client = static_cast<Client *>(*itc);
                if (client->getSocketFd() == res->getSocketFd())
                {
                    client->activeResponse = NULL;
                    mMux->add(*client);
                    break;
                }
                ++itc;
            }

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
        IUpload *upload = qu.front();
        try
        {
            upload->handle();
            if (upload->done())
            {
                mMux->remove(upload);
                IRequest *request = upload->getRequest();

                IResponse *response = new Response(request->getSocket());
                response->setStatusCode(201)
                    .setBody("upload")
                    .build();
                
                mMux->add(*response);
                mResponses.push_back(response);

                std::vector<Client *>::iterator itc = mClients.begin();
                while (itc != mClients.end())
                {
                    Client &client = **itc;
                    if (client.activeUpload == upload)
                    {
                        client.activeUpload = NULL;
                        mMux->add(client);
                        break;
                    }
                    ++itc;
                }

                Client *client = *itc;
                client->status = Client::RECEIVING;
                client->activeResponse = response;

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
            std::cout << "cgi request reading\n"
                      << std::flush;
            req->read();
            if (req->done())
            {
                mMux->remove(req, IMultiplexer::WRITE);
                if (req->getSocketFd() != -1)
                {
                    mMux->remove(req, IMultiplexer::READ);

                    std::vector<Client *>::iterator itc = mClients.begin();
                    while (itc != mClients.end())
                    {
                        Client &client = **itc;
                        if (client.activeProxyPair.request == req)
                        {
                            client.activeProxyPair.request = NULL;
                            mMux->add(client);
                            break;
                        }
                        ++itc;
                    }
                }
                delete req;
            }
        }
        catch (const RequestException &e)
        {
            std::cout << "cgi request error " << e.what() << std::endl
                      << std::flush;
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
            std::cout << "cgi request sending\n"
                      << std::flush;
            req->send();
            if (req->done())
            {
                mMux->remove(req, IMultiplexer::WRITE);
                if (req->getSocketFd() != -1)
                {
                    mMux->remove(req, IMultiplexer::READ);

                    std::vector<Client *>::iterator itc = mClients.begin();
                    while (itc != mClients.end())
                    {
                        Client &client = **itc;
                        if (client.activeProxyPair.request == req)
                        {
                            client.activeProxyPair.request = NULL;
                            mMux->add(client);
                            break;
                        }
                        ++itc;
                    }
                }
                delete req;
            }
        }
        catch (const std::exception &e)
        {
            std::cout << "cgi sending error " << e.what() << std::endl
                      << std::flush;

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
                IResponse *res = new BufferResponse(client.getSocket());

                std::string body = "<center>500</center><br><center>server error</center>";
                res->setStatusCode(500).setHeader("Content-type", "text/html").setBody(body).build();

                mMux->add(*res);
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
            std::cout << "cgi response reading\n"
                      << std::flush;
            res->read();
        }
        catch (const std::exception &e)
        {
            std::cerr << "cgi response error " << e.what() << std::endl;
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
            std::cout << "cgi response sending\n"
                      << std::flush;
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
                        std::cout << "response setting child free" << std::endl;
                        client.activeProxyPair.setChildFree();
                        client.activeProxyPair.response = NULL;
                        mMux->add(client);
                        break;
                    }
                    ++itc;
                }
                if (res->error())
                {
                    Client &client = **itc;
                    IResponse *res = new BufferResponse(client.getSocket());

                    std::string body = "500<br><center>server error</center>";
                    res->setStatusCode(500).setHeader("Content-type", "text/html").setBody(body).build();

                    mMux->add(*res);
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
            std::cout << "cgi response error " << e.what() << std::endl;
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
        mMux->remove(*client.activeResponse);
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

    mMux->remove(client);

    std::vector<Client *>::iterator it = std::find(mClients.begin(), mClients.end(), &client);
    mClients.erase(it);
}