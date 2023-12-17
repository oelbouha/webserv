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

#include <map>
#include <queue>

#include <sys/select.h>
#include <sys/time.h>

#include "IMultiplexer.hpp"
#include "IServerSocket.hpp"

class SelectMultiplexer : public IMultiplexer {
  fd_set mReadfds, mReadfdsTmp;
  fd_set mWritefds, mWritefdsTmp;
  fd_set mExceptfds, mExceptfdsTmp;

  std::map<int, IServerSocket *> serverSockets;
  std::map<int, IClient *> clients;
  std::map<int, IResponse *> responses;

  int mMaxfd;
  int mReadyfdsCount;
  bool fdsChanged;

public:
  typedef std::map<int, IServerSocket *> ServerSockets;
  typedef std::map<int, IClient *> Clients;
  typedef std::map<int, IResponse *> Responses;

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

  void wait(std::size_t timeout);

private:
  void prepareFdSets();
};
#endif
