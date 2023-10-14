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

#include "IServer.hpp"
#include "ServersCluster.hpp"
#include "ConnectionsPool.hpp"

class WebServer
{
    /*  Private members here    */
    WebServer*              mSingleton;
    ServersCluster          mServers;
    ConnectionsPool         mPool;
    

    WebServer();
    WebServer( WebServer& w);
    ~WebServer();

    WebServer&  operator=( WebServer& w );
    
    void    loop();

    public:
        static WebServer*   init();
        void                run();

};
#endif