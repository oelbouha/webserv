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

#include "Config.hpp"
#include "IServer.hpp"
#include "IMultiplexer.hpp"

#include "ServersCluster.hpp"

class WebServer
{
    static WebServer*       mSingleton;

public:
    static WebServer&       getInstace();


private:
    Config*                 mConfig;
    map<string, IServer*>   mServers;//key should be ip:port
    IMultiplexer*           mMx;
    // ServersCluster*         mServers;

    WebServer();
    WebServer( WebServer& w);

    WebServer&  operator=( WebServer& w );

    void    loop();

public:
    WebServer(const Config* aConfig);
    ~WebServer();

    void    start();

};
#endif