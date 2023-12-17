/*                       __          _
 *     __  ___________ _/ /___ ___  (_)
 *    / / / / ___/ __ `/ / __ `__ \/ /
 *   / /_/ (__  ) /_/ / / / / / / / /
 *   \__, /____/\__,_/_/_/ /_/ /_/_/
 *  /____/  User: Youssef Salmi
 *          File: WebServer.hpp
 */

#pragma once
#ifndef WEBSERVER_HPP
#define WEBSERVER_HPP

#include <iostream>
#include <map>
#include <vector>

#include "IMultiplexer.hpp"
#include "IServer.hpp"
#include "src/DataTypes/Config.hpp"

#include "ServersCluster.hpp"

#include "IServerSocket.hpp"

#include "src/Client/Client.hpp"
#include "src/Request/Request.hpp"
#include "src/Response/Response.hpp"
#include "src/Socket/ClientSocket.hpp"
#include "src/Socket/ServerSocket.hpp"

#include "src/Multiplexer/SelectMultiplexer.hpp"

class WebServer {
  const Config*                 mConfig;
  std::vector<IServerSocket *>  mSockets;
  IMultiplexer*                 mMux;
  ServersCluster*               mServers;
  std::vector<Client>           mClients;
  std::queue<IRequest *>        mRequests;
  std::vector<IResponse *>      mResponses;

  ServerSocket mSocket;

  WebServer();

public:
  WebServer(const Config *aConfig);
  ~WebServer();

  void start();
  void loop();
};
#endif
