/*	                     __          _
 *	   __  ___________ _/ /___ ___  (_)
 *	  / / / / ___/ __ `/ / __ `__ \/ /
 *	 / /_/ (__  ) /_/ / / / / / / / /
 *	 \__, /____/\__,_/_/_/ /_/ /_/_/
 *	/____/	User: Youssef Salmi
 *			File: Client.hpp
 */

#pragma once
#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <iostream>
#include <queue>
#include <ctime>

#include "src/Interfaces/IClient.hpp"
#include "src/Interfaces/IClientSocket.hpp"
#include "src/Request/Request.hpp"
#include "src/Route/Upload.hpp"
#include "src/CGI/ProxyPair.hpp"
#include "src/Socket/SocketException.hpp"

#include "Logger.hpp"

class Client : public IClient
{
    int             mIncomingIP;
    int             mIncomingPort;
    IClientSocket*  mSocket;
    Request*        mRequest;
    bool            mKeepAlive;
    time_t          mLastActivityEnd;

public:
    enum  Status
    {
        CONNECTED,
        ACTIVE,
        IDLE
    };

    IResponse*      activeResponse;
    Upload*         activeUpload;
    ProxyPair       activeProxyPair;
    Status          status;
    

public:
    Client(IClientSocket *aSocket, int aIncomingIP, int aIncomingPort);
    ~Client();

    bool  operator==(const IClient& client) const;

    virtual int       getSocketFd() const;
    virtual Request*  getRequest();
    virtual bool      hasRequest() const;
    virtual void      makeRequest();
    bool              hasTimedOut() const;
    void              setResponseHeaders(IResponse* res) const;
    void              setResponseHeaders(IProxyResponse* res) const;
    void              resetTimeout();
    void              setKeepAlive( std::string connection );


private:
    static int KeepAliveCount;
    static int KeepAliveMax;
    static int KeepAliveTimeout;
public:
    static void setKeepAlive(int maxConnections, int timeout);
};
#endif
