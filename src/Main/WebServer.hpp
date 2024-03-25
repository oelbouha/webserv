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
#include "Logger.hpp"

#include "src/DataTypes/Config.hpp"
#include "src/DataTypes/ConfigException.hpp"

#include "src/Interfaces/IServerSocket.hpp"
#include "src/Interfaces/IMultiplexer.hpp"

#include "src/Server/Cluster.hpp"
#include "src/Multiplexer/SelectMultiplexer.hpp"
#include "src/Client/Client.hpp"
#include "src/Request/Request.hpp"
#include "src/Request/BufferRequest.hpp"
#include "src/Socket/ClientSocket.hpp"
#include "src/Socket/ServerSocket.hpp"

#include "Utils.hpp"


class WebServer
{
    Config*                       mConfig;
    std::vector<ServerSocket>     mSockets;
    IMultiplexer*                 mMux;
    Cluster*                mCluster;
    std::vector<Client*>          mClients;


private:
    void  acceptNewClients();
    void  takeAndHandleRequests();
    void  sendResponses();
    void  handleUploads();
    void  readFromReadyProxyRequests();
    void  sendReadyProxyRequests();
    void  readFromReadyProxyResponses();
    void  sendReadyProxyResponses();

    void  handleClientRequest(Client* client, Request* request);
    void  disconnectClient(Client& client);
    
    void  cleanup();

public:
    void  InitiateServersSockets();
    WebServer(Config *aConfig);
    ~WebServer();

    void start();
    void loop();
};
#endif
