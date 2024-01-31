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

WebServer::WebServer() : mConfig(NULL) {}

WebServer::WebServer(const Config *aConfig) : mConfig(aConfig) {
  mMux = new SelectMultiplexer();
}

WebServer::~WebServer() {}

void WebServer::start()
{
  (void)mConfig;

  mSockets.push_back(ServerSocket(utils::ip(0, 0, 0, 0), 8000));
  mSockets.push_back(ServerSocket(utils::ip(0, 0, 0, 0), 8001));
  mSockets.push_back(ServerSocket(utils::ip(0, 0, 0, 0), 8002));
  mSockets.push_back(ServerSocket(utils::ip(0, 0, 0, 0), 8003));
  mSockets.push_back(ServerSocket(utils::ip(0, 0, 0, 0), 8004));

  for (std::vector<ServerSocket>::iterator it = mSockets.begin();
       it != mSockets.end(); ++it)
  {
    it->bind();
    it->listen();
    mMux->add(*it);
    std::cout << "listening on port: " << it->getPort() << std::endl;
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
    // function that loops and close connections to clients that need the connection to be closed
  }
}


void    WebServer::acceptNewClients()
{
    std::queue<IServerSocket *>   qs = mMux->getReadyServerSockets();;
    while (qs.size())
    {
        const IServerSocket&  sock = *(qs.front());

        IClientSocket *clientSock = sock.accept();

        clientSock->setNonBlocking();

        Client* client = new Client(clientSock, sock.getIP(), sock.getPort());

        mClients.push_back(client);

        mMux->add(*client);

        qs.pop();
    }
}

void    WebServer::takeAndHandleRequests()
{
    std::queue<IClient *> qc = mMux->getReadyClients();
    while (qc.size())
    {
      Client *client = static_cast<Client*>(qc.front());

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
        

        /*
        client->set();
        */

        std::string ext;
        if (request->getURI().rfind(".") != std::string::npos)
          ext = request->getURI().substr(request->getURI().rfind("."));

        /*
        
        Result  result = mCluster.handle(request);

        if (result.type == Result::RESPONSE){}
        else if (result.type == Result::PROXY_PAIR){}

        */
          
        if (ext == ".py")
        {
            ProxyPair   pair = mServers.handleCGI(request);

            client->status = Client::EXCHANGING;
            client->activeProxyPair = pair;

            // if (pair.request == NULL || pair.response == NULL) delete make 500 response
            if (pair.request->getSocketFd() != -1)
              mMux->add(pair.request, IMultiplexer::READ);
            else
              mMux->add(*client);
            mMux->add(pair.request, IMultiplexer::WRITE);

            mMux->add(pair.response, IMultiplexer::READ);
            mMux->add(pair.response, IMultiplexer::WRITE);
        }
        else
        {
            IResponse *response = mServers.handle(request);

            // client->setResponseHeaders(response);

            mMux->add(*response);
            mResponses.push_back(response);

            client->status = Client::RECEIVING;
            client->activeResponse = response;

            delete request;
        }
      }

      qc.pop();
    }
}


void    WebServer::sendResponses()
{
  std::queue<IResponse *>       qr = mMux->getReadyResponses();
  while (qr.size()) {
    IResponse *res = qr.front();

    res->send();
    if (res->done())
    {
      mMux->remove(*res);
      std::vector<IResponse*>::iterator   it = std::find(mResponses.begin(), mResponses.end(), res);
      mResponses.erase(it);
      
      std::vector<Client*>::iterator   itc = mClients.begin();
      while (itc != mClients.end())
      {
        Client*  client = static_cast<Client*>(*itc);
        if (client->getSocketFd() == res->getSocketFd()){
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

void    WebServer::readFromReadyProxyRequests()
{
  std::queue<IProxyRequest *>   qpreqs = mMux->getReadyForReadingProxyRequests();
  
  while (qpreqs.size())
  {
    IProxyRequest*  req = qpreqs.front();

    try
    {
        std::cout << "cgi request reading\n" << std::flush;
        req->read();
        if (req->done())
        {
            mMux->remove(req, IMultiplexer::WRITE);
            if (req->getSocketFd() != -1)
            {
                mMux->remove(req, IMultiplexer::READ);

                std::vector<Client*>::iterator   itc = mClients.begin();
                while (itc != mClients.end())
                {
                    Client&  client = **itc;
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
    catch (const RequestException& e)
    {
        std::cout << "cgi request error " << e.what() << std::endl << std::flush;
        if (e.error == RequestException::CONNECTION_CLOSED)
        {
            std::vector<Client*>::iterator   itc = mClients.begin();
            while (itc != mClients.end())
            {
                Client&  client = **itc;
                if (client.activeProxyPair.request == req)
                {
                    disconnectClient(client);
                    break;
                }
                ++itc;
            }
        }
        else {
            // bad request;
        }
    }

    qpreqs.pop();
  }
}

void    WebServer::sendReadyProxyRequests()
{
  std::queue<IProxyRequest *>   qpreqs = mMux->getReadyForWritingProxyRequests();

  while (qpreqs.size())
  {
    IProxyRequest*  req = qpreqs.front();

    try
    {
        std::cout << "cgi request sending\n" << std::flush;
        req->send();
        if (req->done())
        {
            mMux->remove(req, IMultiplexer::WRITE);
            if (req->getSocketFd() != -1)
            {
                mMux->remove(req, IMultiplexer::READ);

                std::vector<Client*>::iterator   itc = mClients.begin();
                while (itc != mClients.end())
                {
                    Client&  client = **itc;
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
    catch (const std::exception& e)
    {
        std::cout << "cgi sending error " << e.what() << std::endl << std::flush;

        std::vector<Client*>::iterator   itc = mClients.begin();
        while (itc != mClients.end())
        {
            Client&  client = **itc;
            if (client.activeProxyPair.request == req)
                break;
            ++itc;
        }
        Client& client = **itc;

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
            IResponse*  res = new Response(client.getSocket());
          
            std::string body = "<center>500</center><br><center>server error</center>";
            res->setStatusCode(500).setHeader("Content-type", "text/html")
                        .setBody(body)
                        .build();
            
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

void    WebServer::readFromReadyProxyResponses()
{
  // cgi progrpams which exited could be detected here
  // need to be handled here
  // clening:
    // request need to be removed
    // generate an error response if possible
  std::queue<IProxyResponse *>  qpres = mMux->getReadyForReadingProxyResponses();
  while (qpres.size())
  {
    IProxyResponse* res = qpres.front();
    try
    {
      std::cout << "cgi response reading\n" << std::flush;
      res->read();
    }
    catch (const std::exception& e)
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

void    WebServer::sendReadyProxyResponses()
{
  // clients who closed connections could be detected here
  // need to be handled here
  // cleaning:
    // client
    // request
    // response
  std::queue<IProxyResponse *>  qpres = mMux->getReadyForWritingProxyResponses();
  while (qpres.size())
  {
    IProxyResponse* res = qpres.front();

    try {
      std::cout << "cgi response sending\n" << std::flush;
      res->send();
      if (res->done() || res->error())
      {
        mMux->remove(res, IMultiplexer::READ);
        mMux->remove(res, IMultiplexer::WRITE);
        std::vector<Client*>::iterator   itc = mClients.begin();
        while (itc != mClients.end())
        {
          Client&  client = **itc;
          if (client.getSocketFd() == res->getSocketFd()){
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
          Client&  client = **itc;
          IResponse*  res = new Response(client.getSocket());
          
          std::string body = "500<br><center>server error</center>";
          res->setStatusCode(500).setHeader("Content-type", "text/html")
                    .setBody(body)
                    .build();
          
          mMux->add(*res);
          mResponses.push_back(res);

          client.status = Client::RECEIVING;
          client.activeResponse = res;

        }
        delete res;
      }
    }
    catch (const std::exception& e)
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




void  WebServer::disconnectClient(Client& client)
{
  if (client.activeResponse)
  {
    mMux->remove(*client.activeResponse);
    std::vector<IResponse*>::iterator   it = std::find(mResponses.begin(), mResponses.end(), client.activeResponse);
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

  std::vector<Client*>::iterator it = std::find(mClients.begin(), mClients.end(), &client);
  mClients.erase(it);
}