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

class Client : public IClient {
  int               mIncomingIP;
  int               mIncomingPort;
  IClientSocket*    mSocket;
  bool              mHasClosedTheConnection;
  bool              mHasRequest;
  IRequest*         mRequest;

public:
  Client(IClientSocket *aSocket, int aIncomingIP, int aIncomingPort);
  ~Client();

  bool  operator==(const IClient& client) const;

  virtual int       getID() const;
  virtual int       getIncomingIP() const;
  virtual int       getIncomingPort() const;

  virtual bool      hasClosedTheConnection() const;
  virtual bool      hasRequest() const;
  virtual void      makeRequest();
  virtual IRequest* getRequest();
};
#endif
