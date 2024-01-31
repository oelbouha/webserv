/*	                     __          _
 *	   __  ___________ _/ /___ ___  (_)
 *	  / / / / ___/ __ `/ / __ `__ \/ /
 *	 / /_/ (__  ) /_/ / / / / / / / /
 *	 \__, /____/\__,_/_/_/ /_/ /_/_/
 *	/____/	User: Youssef Salmi
 *			File: IMultiplexer.hpp
 */

#pragma once
#include "IProxiedResponse.hpp"
#ifndef IMULTIPLEXER_HPP
#define IMULTIPLEXER_HPP

#include <queue>

#include "IClient.hpp"
#include "IResponse.hpp"
#include "IServerSocket.hpp"
#include "IProxyRequest.hpp"
#include "IProxyResponse.hpp"

// implement with time efficiency in mind
// we are only using one thread of execution
class IMultiplexer {
public:
  typedef enum {
    READ,
    WRITE
  } mod_t;

public:
  virtual ~IMultiplexer(){};

  virtual void wait(unsigned long int timeout) = 0;
  virtual bool ready() const = 0;

  // server sockets used only to listen for new Clients
  virtual void add(IServerSocket &aServer) = 0;
  virtual void remove(IServerSocket &aServer) = 0;

  // client sockets used only to listen for new Requests
  virtual void add(IClient &aClient) = 0;
  virtual void remove(IClient &aClient) = 0;

  // response socket used only to listen if the client is ready to receive data
  virtual void add(IResponse &aResponse) = 0;
  virtual void remove(IResponse &aResponse) = 0;

  virtual std::queue<IServerSocket *> getReadyServerSockets() const = 0;
  virtual std::queue<IClient *> getReadyClients() const = 0;
  virtual std::queue<IResponse *> getReadyResponses() const = 0;

  // cgi
  virtual void  add(IProxyRequest*  aRequest, mod_t mod) = 0;
  virtual void  add(IProxyResponse*  aResponse, mod_t mod) = 0;
  virtual void  remove(IProxyRequest*  aRequest, mod_t mod) = 0;
  virtual void  remove(IProxyResponse*  aResponse, mod_t mod) = 0;

  virtual std::queue<IProxyRequest*>  getReadyForReadingProxyRequests() const = 0;
  virtual std::queue<IProxyRequest*>  getReadyForWritingProxyRequests() const = 0;
  virtual std::queue<IProxyResponse*>  getReadyForReadingProxyResponses() const = 0;
  virtual std::queue<IProxyResponse*>  getReadyForWritingProxyResponses() const = 0;

};

/*
  add(ProxyPair*)
  {
    add request input for write events
    add client socket for read events

    add response output for read events
    add client socket for write events

    pb:
      client already registered for read events
  }
*/

#endif
