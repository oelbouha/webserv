/*	                     __          _
 *	   __  ___________ _/ /___ ___  (_)
 *	  / / / / ___/ __ `/ / __ `__ \/ /
 *	 / /_/ (__  ) /_/ / / / / / / / /
 *	 \__, /____/\__,_/_/_/ /_/ /_/_/
 *	/____/	User: Youssef Salmi
 *			File: SelectMultiplexer.hpp
 */

#pragma once
#ifndef SELECTMULTIPLEXER_HPP
#define SELECTMULTIPLEXER_HPP

#include <iostream>
#include <map>
#include <queue>

#include <sys/select.h>
#include <sys/time.h>

#include "IMultiplexer.hpp"


class SelectMultiplexer : public IMultiplexer {
  fd_set mReadfds, mReadfdsTmp;
  fd_set mWritefds, mWritefdsTmp;

  std::map<int, IServerSocket *>  serverSockets;
  std::map<int, IClient *>        clients;
  std::map<int, IResponse *>      responses;
  std::map<int, IUpload *>        uploads;
  std::map<int, IProxyRequest *>  proxyRequests;
  std::map<int, IProxyResponse *> proxyResponses;

  int mMaxfd;
  int mReadyfdsCount;

public:
  typedef std::map<int, IServerSocket *>  ServerSockets;
  typedef std::map<int, IClient *>        Clients;
  typedef std::map<int, IResponse *>      Responses;
  typedef std::map<int, IUpload *>      Uploads;
  typedef std::map<int, IProxyRequest *>  ProxyRequests;
  typedef std::map<int, IProxyResponse *> ProxyResponses;

public:
  SelectMultiplexer();
  SelectMultiplexer(const SelectMultiplexer &s);
  ~SelectMultiplexer();

public:
  SelectMultiplexer &operator=(const SelectMultiplexer &s);

public:
  void add(IServerSocket &sock);
  void remove(IServerSocket &sock);
  std::queue<IServerSocket *> getReadyServerSockets() const;

  void add(IClient &client);
  void remove(IClient &client);
  std::queue<IClient *> getReadyClients() const;

  void add(IResponse &res);
  void remove(IResponse &res);
  std::queue<IResponse *> getReadyResponses() const;

  void                    add(IUpload *upload);
  void                    remove(IUpload *upload);
  std::queue<IUpload *>   getReadyUploads() const;

  void  add(IProxyRequest*  aRequest, mod_t mod);
  void  add(IProxyResponse*  aResponse, mod_t mod);
  void  remove(IProxyRequest*  aRequest, mod_t mod);
  void  remove(IProxyResponse*  aResponse, mod_t mod);


  std::queue<IProxyRequest*>  getReadyForReadingProxyRequests() const;
  std::queue<IProxyRequest*>  getReadyForWritingProxyRequests() const;
  std::queue<IProxyResponse*>  getReadyForReadingProxyResponses() const;
  std::queue<IProxyResponse*>  getReadyForWritingProxyResponses() const;

  void wait(std::size_t timeout);
  bool ready() const;

private:
  void  updateMaxFd();
};
#endif
