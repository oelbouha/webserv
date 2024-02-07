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
#include <vector>
#include "Utils.hpp"

#include "src/DataTypes/Config.hpp"

#include "src/Interfaces/IServerSocket.hpp"
#include "src/Interfaces/IMultiplexer.hpp"

#include "src/Server/ServerCluster.hpp"
#include "src/Multiplexer/SelectMultiplexer.hpp"
#include "src/Client/Client.hpp"
#include "src/Request/Request.hpp"
#include "src/Response/Response.hpp"
#include "src/Socket/ClientSocket.hpp"
#include "src/Socket/ServerSocket.hpp"
#include "src/DataTypes/ConfigException.hpp"

#include "src/Request/BufferRequest.hpp"


class WebServer
{
  Config*                       mConfig;
  std::vector<ServerSocket>     mSockets;
  IMultiplexer*                 mMux;
  ServerCluster                 *mServers;
  std::vector<Client*>          mClients;
  std::vector<IResponse *>      mResponses;
  // unsigned int                  mAliveClientsCount;
  // unsigned int                  mAliveClientMax;
  // unsigned int                  mAliveTimeout;


private:
  void  acceptNewClients();
  void  takeAndHandleRequests();
  void  sendResponses();

  void  handleUploads();

  void  readFromReadyProxyRequests();
  void  sendReadyProxyRequests();
  void  readFromReadyProxyResponses();
  void  sendReadyProxyResponses();

  void  disconnectClient(Client& client);

public:
  void  InitiateServersSockets();
  WebServer(Config *aConfig);
  ~WebServer();

  void start();
  void loop();
};
#endif
