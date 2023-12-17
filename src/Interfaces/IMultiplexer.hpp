/*	                     __          _
 *	   __  ___________ _/ /___ ___  (_)
 *	  / / / / ___/ __ `/ / __ `__ \/ /
 *	 / /_/ (__  ) /_/ / / / / / / / /
 *	 \__, /____/\__,_/_/_/ /_/ /_/_/
 *	/____/	User: Youssef Salmi
 *			File: IMultiplexer.hpp
 */

#pragma once
#ifndef IMULTIPLEXER_HPP
#define IMULTIPLEXER_HPP

#include <queue>

#include "IClient.hpp"
#include "IResponse.hpp"
#include "IServerSocket.hpp"

// implement with time efficiency in mind
// we are only using one thread of execution
class IMultiplexer {
public:
  virtual ~IMultiplexer(){};

  // server sockets used only to listen for new Clients
  virtual void add(IServerSocket &aServer) = 0;
  virtual void remove(IServerSocket &aServer) = 0;

  // client sockets used only to listen for new Requests
  virtual void add(IClient &aClient) = 0;
  virtual void remove(IClient &aClient) = 0;

  // response socket used only to listen if the client is ready to receive data
  virtual void add(IResponse &aResponse) = 0;
  virtual void remove(IResponse &aResponse) = 0;

  virtual void wait(unsigned long int timeout) = 0;

  virtual std::queue<IServerSocket *> getReadyServerSockets() const = 0;
  virtual std::queue<IClient *> getReadyClients() const = 0;
  virtual std::queue<IResponse *> getReadyResponses() const = 0;
};

#endif
