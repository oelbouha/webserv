/*	                     __          _
 *	   __  ___________ _/ /___ ___  (_)
 *	  / / / / ___/ __ `/ / __ `__ \/ /
 *	 / /_/ (__  ) /_/ / / / / / / / /
 *	 \__, /____/\__,_/_/_/ /_/ /_/_/
 *	/____/	User: Youssef Salmi
 *			File: Client.hpp
 */

#pragma once
#include <queue>
#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <iostream>

#include "IClient.hpp"
#include "IClientSocket.hpp"

#include "src/Socket/SocketException.hpp"

#include "src/Request/Request.hpp"
#include "src/Response/Response.hpp"

#include "src/CGI/ProxyPair.hpp"

class Client : public IClient
{
  int                   mIncomingIP;
  int                   mIncomingPort;
  IClientSocket*        mSocket;
  std::queue<IRequest*> mRequests;


public:
  enum  Status
  {
    CONNECTED,
    EXCHANGING,
    RECEIVING,
    DISCONNECTED
  };

  IResponse*  activeResponse;
  ProxyPair   activeProxyPair;
  // active upload
  Status      status;


public:
  Client(IClientSocket *aSocket, int aIncomingIP, int aIncomingPort);
  ~Client();

  bool  operator==(const IClient& client) const;

  virtual int       getSocketFd() const;
  virtual int       getIncomingIP() const;
  virtual int       getIncomingPort() const;
  virtual IRequest* getRequest();

  virtual bool      hasRequest() const;
  virtual void      makeRequest();

  virtual void      dump();

  virtual const IClientSocket&  getSocket() const;
};
#endif
