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

WebServer::WebServer( Config *aConfig) : mConfig(aConfig) {
  // mMux = new SelectMultiplexer();
  mMux = new KqueueMultiplexer();
  mServers = new ServerCluster(mConfig);
}

WebServer::~WebServer() {}

void WebServer::start() {
  (void)mConfig;

  mSockets.push_back(ServerSocket(utils::ip(0, 0, 0, 0), 8000));
  mSockets.push_back(ServerSocket(utils::ip(0, 0, 0, 0), 8001));
  mSockets.push_back(ServerSocket(utils::ip(0, 0, 0, 0), 8002));
  mSockets.push_back(ServerSocket(utils::ip(0, 0, 0, 0), 8003));
  mSockets.push_back(ServerSocket(utils::ip(0, 0, 0, 0), 8004));

  for (std::vector<ServerSocket>::iterator it = mSockets.begin();
       it != mSockets.end(); ++it) {
    it->bind();
    it->listen();
    mMux->add(*it);
    std::cout << "listening on port: " << it->getPort() << std::endl;
  }
}

void WebServer::loop() {
  std::queue<IServerSocket *> qs;
  std::queue<IClient *> qc;
  std::queue<IResponse *> qr;

  while (true) {
    std::cout << "." << std::flush;
    mMux->wait(10 * 1000000); // 10 seconds

    qs = mMux->getReadyServerSockets();
    qc = mMux->getReadyClients();
    qr = mMux->getReadyResponses();

    // if (qs.size() == 0 && qc.size() == 0 && qr.size() == 0)
    // {
    //     std::cout << "." << std::flush;
    //     continue;
    // }

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
    // std::cout << "Accepting clients...." << std::flush;
    while (qs.size()) {
        const IServerSocket *sock = qs.front();
        IClientSocket *clientSock = sock->accept();
        clientSock->setNonBlocking();
        Client client(clientSock, sock->getIP(), sock->getPort());

        mClients.push_back(client);
        mMux->add(mClients.back());
        qs.pop();

    }
    // std::cout << "Done.\n" << std::flush;
}

void    WebServer::takeAndHandleRequests(std::queue<IClient*>& qc)
{
    while (qc.size()) {
      IClient *client = qc.front();

      client->makeRequest();
      if (client->hasClosedTheConnection()) {
          std::cout << "client closed connection.\n" << std::flush;
          mMux->remove(*client);
          std::vector<Client>::iterator it = std::find(mClients.begin(),
                                                        mClients.end(),
                                                        *client);
          mClients.erase(it);
      }
      else if (client->hasRequest()) {
        std::cout << "client has request.\n" << std::flush;
        IRequest *request = client->getRequest();
        // request->dump(true);
        // std::cout << "handling request.\n" << std::flush;
        IResponse *response = mServers->handle(request);
        // ResponseWrapper  *res = mServers.handle(request);
        // if (res->getType() == SIMPLE_RESPONSE)
        // std::cout << "start sending.\n" << std::flush;
        response->startSending();
        mMux->add(*response);
        std::cout << "client request has been handeled.\n" << std::flush;
      }
      qc.pop();
    }
}

void    WebServer::sendResponses(std::queue<IResponse*>& qr)
{
    while (qr.size()) {
      IResponse *res = qr.front();
        std::cout << "sending response to client .....\n" << std::flush;

      /*
      for (std::vector<Client>::iterator it = mClients.begin(); it != mClients.end(); ++it)
      {
          if (it->getID() == res->getID())
          {
              mMux->remove(*res);
          //    delete res;
          }
      }*/

      res->send();
      if (res->isSendingComplete())
      {
        mMux->remove(*res);
        // delete res;
      }
      qr.pop();
    } 
}
