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

#include "src/DataTypes/Config.hpp"
#include "IServer.hpp"
#include "IMultiplexer.hpp"

#include "ServersCluster.hpp"

#include "IServerSocket.hpp"

#include "src/Socket/ServerSocket.hpp"
#include "src/Socket/ClientSocket.hpp"
#include "src/Request/Request.hpp"
#include "src/Response/Response.hpp"


class WebServer
{
    static WebServer*       mSingleton;

public:
    static WebServer&       getInstace();


private:
    const Config*           mConfig;
    // map<string, IServer*>   mServers;//key should be ip:port
    ServersCluster*         mServers;
    vector<IServerSocket*>  mSockets;
    IMultiplexer*           mMx;

    ServerSocket            mSocket;

    WebServer();
    WebServer( const WebServer& w);

    WebServer&  operator=( const WebServer& w );


public:
    WebServer(const Config* aConfig);
    ~WebServer();

    void    start();
    void    loop();

};
#endif