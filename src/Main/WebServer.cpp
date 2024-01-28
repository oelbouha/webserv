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

void  setMimetypes(Config * config) {
  Config* cluster = config->getBlockConfig("cluster").front();
  
  if (cluster->hasBlock("mime_types") == false) {
    std::map<string, string> mimetypes;

    mimetypes["html"] = "text/html";
    mimetypes["css"] = "text/css";
    mimetypes["xml"] = "text/xml";
    mimetypes["javascript"] = "application/javascript";
    mimetypes["rss+xml"] = "application/rss+xml";
    mimetypes["mathml"] = "text/mathml";
    mimetypes["plain"] = "text/plain";
    
    // images mimetypes
    mimetypes["gif"] = "image/gif";
    mimetypes["jpeg"] = "image/jpeg";
    mimetypes["png"] = "image/png";
    mimetypes["svg+xml"] = "image/svg+xml";
    mimetypes["tiff"] = "image/tiff";
    mimetypes["vnd.wap.wbmp"] = "image/vnd.wap.wbmp";
    mimetypes["webp"] = "image/webp";
    mimetypes["x-icon"] = "image/x-icon";
    mimetypes["x-jng"] = "image/x-jng";
    mimetypes["x-ms-bmp"] = "image/x-ms-bmp";

    // video mimetypes
    mimetypes["3gpp"] = "video/3gpp";
    mimetypes["mp4"] = "video/mp4";
    mimetypes["mpeg"] = "video/mpeg";
    mimetypes["quicktime"] = "video/quicktime";
    mimetypes["webm"] = "video/webm";

    Config * mimeConfig = new Config();
    std::map<string, string>::iterator it = mimetypes.begin();
    while (it != mimetypes.end()) {
      mimeConfig->addInline(it->first, it->second);
      ++it;
    }
    mimeConfig->addInline("default_mime", "text/plain");
    cluster->addBlock("mime_type", mimeConfig);
  }
  MimeTypes::setMimeTypes(cluster->getBlockConfig("mime_type").front());
}

WebServer::WebServer() : mConfig(NULL) {}

WebServer::WebServer( Config *aConfig) : mConfig(aConfig) {
  setMimetypes(aConfig);
  mMux = new SelectMultiplexer();
}

void  WebServer::SetDefaultIfNotExist() {
  if (mConfig->hasBlock("cluster") == false)
    throw ConfigException("Webserver: Invalid Config No Cluster Found ", "cluster", "Empty");

  Config *cluster = mConfig->getBlockConfigIfExist("cluster").front();
    
  if (cluster->hasInline("keep_alive") == false)
    cluster->addInline("keep_alive", "100");
    
  std::vector<Config*> servers = cluster->getBlockConfigIfExist("server");
  std::vector<Config*>::iterator it = servers.begin();
  
  if (cluster->hasBlock("server") == false)
    throw ConfigException("Webserver: No Server Found", "server", "Empty");

  while (it != servers.end()) {
      Config* server = *it;
    if (server->hasInline("host") == false)
      server->addInline("host", "0.0.0.0");
    if (server->hasInline("port") == false)
      server->addInline("port", "8080");
    ++it;
  }
}

void  WebServer::InitiateServersSockets() {
  SetDefaultIfNotExist();

  Config *cluster = mConfig->getBlockConfigIfExist("cluster").front();

  cluster->IsValidDirective("keep_alive");
  
  std::vector<Config*> servers = cluster->getBlockConfigIfExist("server");
  std::vector<Config*>::iterator it = servers.begin();

  while (it != servers.end()) {
      Config* server = *it;
      
      std::string port = server->getInlineConfigIfExist("port");
      std::vector<string> ports = utils::SplitString(port, ' ');

      std::string host = server->getInlineConfigIfExist("host");
      server->IsValidDirective("host");
      if (host == "localhost")
        host = "127.0.0.1";
      unsigned int Ip = utils::ipToUint(host);

      std::vector<string>::iterator cur = ports.begin();
      while (cur != ports.end())
      {
        server->IsValidDirective("port");
        unsigned int Port = std::stod(*cur, NULL);
        mSockets.push_back(ServerSocket(Ip, Port));
        ++cur;
      }
      ports.clear();
    ++it;
  }
}

WebServer::~WebServer() {
  delete mMux;
  delete mServers;
}

void WebServer::start() {
  InitiateServersSockets();
  mServers = new ServerCluster(mConfig);

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
    mMux->wait(10 * 1000000);

    qs = mMux->getReadyServerSockets();
    qc = mMux->getReadyClients();
    qr = mMux->getReadyResponses();

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
        IResponse *response = mServers->handle(*request);
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
