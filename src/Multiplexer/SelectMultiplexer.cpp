/*	                     __          _
 *	   __  ___________ _/ /___ ___  (_)
 *	  / / / / ___/ __ `/ / __ `__ \/ /
 *	 / /_/ (__  ) /_/ / / / / / / / /
 *	 \__, /____/\__,_/_/_/ /_/ /_/_/
 *	/____/	User: Youssef Salmi
 *			File: SelectMultiplexer.cpp
 */

#include "SelectMultiplexer.hpp"
#include <iostream>

SelectMultiplexer::SelectMultiplexer() : mMaxfd(1024), fdsChanged(true) {}

SelectMultiplexer::SelectMultiplexer(const SelectMultiplexer &s) { (void)s; }

SelectMultiplexer::~SelectMultiplexer() {}

SelectMultiplexer &SelectMultiplexer::operator=(const SelectMultiplexer &s) {
  (void)s;
  return (*this);
}

/******************************************************************************
         Listening Sockets
******************************************************************************/

void SelectMultiplexer::add(IServerSocket &sock) {
  serverSockets[sock.getID()] = &sock;
  mMaxfd = mMaxfd >= sock.getID() ? mMaxfd : sock.getID() + 1;
  fdsChanged = true;
}

void SelectMultiplexer::remove(IServerSocket &sock) {
  ServerSockets::iterator it = serverSockets.find(sock.getID());

  if (it != serverSockets.end())
    serverSockets.erase(it);
  fdsChanged = true;
}

std::queue<IServerSocket *>
SelectMultiplexer::getReadyServerSockets() const {
  std::queue<IServerSocket *> ret;

  if (mReadyfdsCount == 0)
    return (ret);

  ServerSockets::const_iterator its = serverSockets.begin();

  while (its != serverSockets.end()) {
    if (FD_ISSET(its->first, &mReadfdsTmp))
      ret.push(its->second);
    ++its;
  }
  return (ret);
}

/******************************************************************************
          Clients
******************************************************************************/

void SelectMultiplexer::add(IClient &client) {
  clients[client.getID()] = &client;
  mMaxfd = mMaxfd >= client.getID() ? mMaxfd : client.getID() + 1;
  fdsChanged = true;
}

void SelectMultiplexer::remove(IClient &client) {
  Clients::iterator it = clients.find(client.getID());

  if (it != clients.end())
    clients.erase(it);
  fdsChanged = true;
}

std::queue<IClient *> SelectMultiplexer::getReadyClients() const {
  std::queue<IClient *> ret;

  if (mReadyfdsCount == 0)
    return (ret);

  Clients::const_iterator itc = clients.begin();

  while (itc != clients.end()) {
    if (FD_ISSET(itc->first, &mReadfdsTmp))
      ret.push(itc->second);
    ++itc;
  }
  return (ret);
}

/******************************************************************************
         Responses
******************************************************************************/

void SelectMultiplexer::add(IResponse &res) {
  responses[res.getID()] = &res;
  mMaxfd = mMaxfd >= res.getID() ? mMaxfd : res.getID() + 1;
  fdsChanged = true;
}

void SelectMultiplexer::remove(IResponse &res) {
  Responses::iterator it = responses.find(res.getID());

  if (it != responses.end())
    responses.erase(it);
  fdsChanged = true;
}

std::queue<IResponse *> SelectMultiplexer::getReadyResponses() const {
  std::queue<IResponse *> ret;

  if (mReadyfdsCount == 0)
    return (ret);

  Responses::const_iterator itr = responses.begin();

  while (itr != responses.end()) {
    if (FD_ISSET(itr->first, &mWritefdsTmp))
      ret.push(itr->second);
    ++itr;
  }
  return (ret);
}

/******************************************************************************
         Wait
******************************************************************************/

void SelectMultiplexer::wait(std::size_t timeout_us) {
  prepareFdSets();

  mReadfdsTmp = mReadfds;
  mWritefdsTmp = mWritefds;
  mExceptfdsTmp = mExceptfds;

  struct timeval timeout;
  timeout.tv_sec = static_cast<long>(timeout_us / 1000000);
  timeout.tv_usec = static_cast<long>(timeout_us % 1000000);

  if (responses.size())
    mReadyfdsCount =
        ::select(mMaxfd, &mReadfdsTmp, &mWritefdsTmp, &mExceptfdsTmp, &timeout);
  else
    mReadyfdsCount =
        ::select(mMaxfd, &mReadfdsTmp, NULL, &mExceptfdsTmp, &timeout);
}

void SelectMultiplexer::prepareFdSets() {
  if (fdsChanged == false)
    return;

  FD_ZERO(&mReadfds);
  FD_ZERO(&mWritefds);
  FD_ZERO(&mExceptfds);

  ServerSockets::iterator its = serverSockets.begin();
  while (its != serverSockets.end()) {
    FD_SET(its->first, &mReadfds);
    FD_SET(its->first, &mExceptfds);
    ++its;
  }
  Clients::iterator itc = clients.begin();
  while (itc != clients.end()) {
    FD_SET(itc->first, &mReadfds);
    FD_SET(itc->first, &mExceptfds);
    ++itc;
  }

  Responses::iterator itr = responses.begin();
  while (itr != responses.end()) {
    FD_SET(itr->first, &mWritefds);
    FD_SET(itr->first, &mExceptfds);
    ++itr;
  }

  fdsChanged = false;
}
