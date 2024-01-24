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
  std::queue<IServerSocket *> qs;
  std::queue<IClient *>       qc;
  std::queue<IResponse *>     qr;

  while (true) 
  {
    mMux->wait(1 * 1000000); // 10 seconds

    qs = mMux->getReadyServerSockets();
    qc = mMux->getReadyClients();
    qr = mMux->getReadyResponses();

    if (qs.size() == 0 && qc.size() == 0 && qr.size() == 0)
    {
        std::cout << "." << std::flush;
        continue;
    }

    std::cout << std::endl;
    std::cout << "qs: " << qs.size();
    std::cout << " qc: " << qc.size();
    std::cout << " qr: " << qr.size() << std::endl << std::flush;

    acceptNewClients(qs);
    takeAndHandleRequests(qc);
    sendResponses(qr);
  }
}


void    WebServer::acceptNewClients(std::queue<IServerSocket*>& qs)
{
    while (qs.size()) {
        const IServerSocket *sock = qs.front();
        std::cout << "accepting from : " << sock->getSocketFd() << std::endl;
        IClientSocket *clientSock = sock->accept();
        clientSock->setNonBlocking();
        Client* client = new Client(clientSock, sock->getIP(), sock->getPort());
        std::cout << "accepted\n" << std::flush;


        mClients.push_back(client);
        mMux->add(*client);
        qs.pop();
    }
}

void    WebServer::takeAndHandleRequests(std::queue<IClient*>& qc)
{
    while (qc.size()) {
      IClient *client = qc.front();


      // if (client in cgi)
      //  set request active
      //  else

  /* */     
          try {
            client->dump();
            if (client->hasClosedTheConnection())
            {
              mMux->remove(*client);
              std::vector<Client*>::iterator it = std::find(mClients.begin(),
                                                        mClients.end(),
                                                        client);
              mClients.erase(it);
                
                for (std::vector<IResponse*>::iterator it = mResponses.begin();
                        it != mResponses.end(); ++it)
                {
                    IResponse*  res = *it;
                    if (res->getSocketFd() == client->getSocketFd())
                    {
                        mResponses.erase(it);
                        delete res;
                        --it;
                    }
                }

              delete client;
            }
          }
          catch(int& e){
              IResponse*  res  = new Response(client->getSocket());

              std::string resStr = "Success\n";
              res->setStatusCode(200)
                  .setHeader("content-length", std::to_string((int)resStr.length()))
                  .setHeader("content-type", "text/plain")
                  .setBody(resStr)
                  .build();
              mMux->add(*res);
              mResponses.push_back(res);
          }
          catch(const std::exception& e)
          {
            mMux->remove(*client);
            std::vector<Client*>::iterator it = std::find(mClients.begin(),
                                                      mClients.end(),
                                                      client);
            mClients.erase(it);
            delete client;
          }
          qc.pop();
          continue;
  /**/
      client->makeRequest();

      if (client->hasClosedTheConnection()) {
          mMux->remove(*client);
          std::vector<Client*>::iterator it = std::find(mClients.begin(),
                                                        mClients.end(),
                                                        client);
          mClients.erase(it);
          delete client;
      }
      else if (client->hasRequest()) {
        IRequest *request = client->getRequest();
        if (false && request->getURI() == "/")
        {
            std::cout << "request to cgi\n" << std::flush;
            IProxiedResponse*   response = mServers.handleCGI(request);

            (void)response;

            mMux->add(*response);
        }
        else
        {
            std::cout << "requesting file\n" << std::flush;
            IResponse *response = mServers.handle(request);
            mMux->add(*response);
            std::cout << "request handled - response ready\n" << std::flush;
        }
        std::cout << "client request has been handeled.\n" << std::flush;
      }

      qc.pop();
    }
}

void    WebServer::sendResponses(std::queue<IResponse*>& qr)
{
    while (qr.size()) {
      IResponse *res = qr.front();

      /*
      for (std::vector<Client>::iterator it = mClients.begin(); it != mClients.end(); ++it)
      {
          if (it->getSocketFd() == res->getSocketFd())
          {
              mMux->remove(*res);
          //    delete res;
          }
      }*/

      res->send();
      if (res->isSendingComplete())
      {
        mMux->remove(*res);
        std::vector<IResponse*>::iterator   it = std::find(mResponses.begin(),
                mResponses.end(), res);
        mResponses.erase(it);
        delete res;
      }
      qr.pop();
    }
 
}

void    readFromReadyProxyRequests(std::queue<ProxyPair*> qProxyPair)
{
  // loop and read
    // if client closed connection
      // remove proxypair from webserv loop
}

void    sendReadyProxyRequests(std::queue<ProxyPair*> qProxyPairs)
{
  // loop and write
    // if cgi program exited generate an error page
  catch (ProxyPair::Error)
  {
    pair->generateErrorResponse();
  }
}

void    readFromReadyProxyResponses(std::queue<ProxyPair*> qProxyPair)
{
  // read;
  // if return is -1
      // if already sent header
          // remove proxypair from webserver loop
      // else
          // build error Response
  // check end of header (\r\n\r\n or \n\n)
      // build response
}

void    sendReadyProxyResponses(std::queue<ProxyPair*> qProxyPair)
{
  // if built and buffer not empty
    // send from buffer
  // else
    // return;
}