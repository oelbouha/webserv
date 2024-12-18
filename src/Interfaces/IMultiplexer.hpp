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
#include "IProxyRequest.hpp"
#include "IProxyResponse.hpp"
#include "IUpload.hpp"

class IMultiplexer {
public:
  typedef enum {
    READ,
    WRITE,
  } mod_t;

public:
  virtual ~IMultiplexer(){};

  virtual void wait(unsigned long int timeout) = 0;
  virtual bool ready() const = 0;

  virtual void add(IServerSocket *aServer) = 0;
  virtual void remove(IServerSocket *aServer) = 0;

  virtual void add(IClient *aClient) = 0;
  virtual void remove(IClient *aClient) = 0;

  virtual void add(IResponse *aResponse) = 0;
  virtual void remove(IResponse *aResponse) = 0;

  // upload 
  virtual void add(IUpload *upload) = 0;
  virtual void remove(IUpload *upload) = 0;

  virtual std::queue<IUpload *> getReadyUploads() const = 0;

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

#endif
