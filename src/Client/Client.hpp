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

#include "IClient.hpp"
#include "IClientSocket.hpp"

#include "src/Request/Request.hpp"
#include "src/Response/Response.hpp"
#include "src/Route/Upload.hpp"
#include "src/CGI/ProxyPair.hpp"

#include "src/Socket/SocketException.hpp"


#include "Logger.hpp"

class Client : public IClient
{
    int             mIncomingIP;
    int             mIncomingPort;
    IClientSocket*  mSocket;
    IRequest*       mRequest;
    bool            mKeepAlive;



public:
    enum  Status
    {
        CONNECTED,
        IDLE,
        EXCHANGING,
        RECEIVING,
        DISCONNECTED
    };

    IResponse*      activeResponse;
    Upload*         activeUpload;
    ProxyPair       activeProxyPair;

    Status      status;
    time_t      lastActivityEnd;

public:
    Client(IClientSocket *aSocket, int aIncomingIP, int aIncomingPort);
    ~Client();

    bool  operator==(const IClient& client) const;

    virtual int       getSocketFd() const;
    virtual IRequest* getRequest();
    virtual bool      hasRequest() const;
    virtual void      makeRequest();
    bool              hasTimedOut() const;
    bool              isKeptAlive() const;
    void              setResponseHeaders(IResponse* res) const;
    void              setResponseHeaders(IProxyResponse* res) const;

    virtual const IClientSocket&  getSocket() const;

private:
    static int KeepAliveCount;
    static int KeepAliveMax;
    static int KeepAliveTimeout;

public:
    static void setKeepAlive(int maxConnections, int timeout);
};
#endif
