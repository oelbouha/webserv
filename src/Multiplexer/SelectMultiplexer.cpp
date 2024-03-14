/*	                     __          _
 *	   __  ___________ _/ /___ ___  (_)
 *	  / / / / ___/ __ `/ / __ `__ \/ /
 *	 / /_/ (__  ) /_/ / / / / / / / /
 *	 \__, /____/\__,_/_/_/ /_/ /_/_/
 *	/____/	User: Youssef Salmi
 *			File: SelectMultiplexer.cpp
 */

#include "SelectMultiplexer.hpp"

SelectMultiplexer::SelectMultiplexer() : mMaxfd(0), mReadyfdsCount(0)
{
  FD_ZERO(&mReadfds);
  FD_ZERO(&mWritefds);
}

SelectMultiplexer::SelectMultiplexer(const SelectMultiplexer &s) { (void)s; }

SelectMultiplexer::~SelectMultiplexer() {}

SelectMultiplexer &SelectMultiplexer::operator=(const SelectMultiplexer &s) {
  (void)s;
  return (*this);
}

/******************************************************************************
         Listening Sockets
******************************************************************************/

void SelectMultiplexer::add(IServerSocket *sock)
{
  serverSockets[sock->getSocketFd()] = sock;
  FD_SET(sock->getSocketFd(), &mReadfds);
  mMaxfd = mMaxfd > sock->getSocketFd() ? mMaxfd : sock->getSocketFd() + 1;
}

void SelectMultiplexer::remove(IServerSocket *sock)
{
  ServerSockets::iterator it = serverSockets.find(sock->getSocketFd());

  if (it != serverSockets.end())
  {
    serverSockets.erase(it);
    FD_CLR(sock->getSocketFd(), &mReadfds);
    if (sock->getSocketFd() == mMaxfd - 1)
      updateMaxFd();
  }
}

std::queue<IServerSocket *>
SelectMultiplexer::getReadyServerSockets() const
{
  std::queue<IServerSocket *> ret;

  if (mReadyfdsCount == 0)
    return (ret);

  ServerSockets::const_iterator its = serverSockets.begin();

  while (its != serverSockets.end())
  {
    if (FD_ISSET(its->first, &mReadfdsTmp))
      ret.push(its->second);
    ++its;
  }
  return (ret);
}

/******************************************************************************
          Clients
******************************************************************************/

void SelectMultiplexer::add(IClient *client)
{
  clients[client->getSocketFd()] = client;
  FD_SET(client->getSocketFd(), &mReadfds);
  mMaxfd = mMaxfd > client->getSocketFd() ? mMaxfd : client->getSocketFd() + 1;
}

void SelectMultiplexer::remove(IClient *client)
{
  Clients::iterator it = clients.find(client->getSocketFd());

  if (it != clients.end())
  {
    clients.erase(it);
    FD_CLR(client->getSocketFd(), &mReadfds);
    if (client->getSocketFd() == mMaxfd - 1)
      updateMaxFd();
  }
}

std::queue<IClient *> SelectMultiplexer::getReadyClients() const
{
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

void SelectMultiplexer::add(IResponse *res)
{
  responses[res->getSocketFd()] = res;
  FD_SET(res->getSocketFd(), &mWritefds);
  mMaxfd = mMaxfd > res->getSocketFd() ? mMaxfd : res->getSocketFd() + 1;
}

void SelectMultiplexer::remove(IResponse *res)
{
  Responses::iterator it = responses.find(res->getSocketFd());

  if (it != responses.end())
  {
    responses.erase(it);
    FD_CLR(res->getSocketFd(), &mWritefds);
    if (res->getSocketFd() == mMaxfd - 1)
      updateMaxFd();
  }
}

std::queue<IResponse *> SelectMultiplexer::getReadyResponses() const
{
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
         UPLOAD
******************************************************************************/

void  SelectMultiplexer::add(IUpload *upload)
{
  uploads[upload->getSocketFd()] = upload;
  FD_SET(upload->getSocketFd(), &mReadfds);
  mMaxfd = mMaxfd > upload->getSocketFd() ? mMaxfd : upload->getSocketFd() + 1;
}

void  SelectMultiplexer::remove(IUpload *upload)
{
  Uploads::iterator it = uploads.find(upload->getSocketFd());

  if (it != uploads.end())
  {
    uploads.erase(it);
    FD_CLR(upload->getSocketFd(), &mWritefds);

     if (upload->getSocketFd() == mMaxfd - 1)
      updateMaxFd();
  }
}

std::queue<IUpload *>   SelectMultiplexer::getReadyUploads() const 
{
  std::queue<IUpload *> ret;

  if (mReadyfdsCount == 0)
    return (ret);

  Uploads::const_iterator itu = uploads.begin();

  while (itu != uploads.end()) {
    if (FD_ISSET(itu->first, &mReadfdsTmp))
      ret.push(itu->second);
    ++itu;
  }
  return (ret);
}

/******************************************************************************
         CGI
******************************************************************************/

void  SelectMultiplexer::add(IProxyRequest* req, IMultiplexer::mod_t mod)
{
    proxyRequests[req->getOutputFd()] = req;
    if (mod == IMultiplexer::READ)
    {
        if (req->getSocketFd() == -1) return;

        FD_SET(req->getSocketFd(), &mReadfds);
        mMaxfd = mMaxfd > req->getSocketFd() ? mMaxfd : req->getSocketFd() + 1;

        Logger::debug (req->getSocketFd())(" : (r) added").flush();
        return;
    }

    FD_SET(req->getOutputFd(), &mWritefds);
    mMaxfd = mMaxfd > req->getOutputFd() ? mMaxfd : req->getOutputFd() + 1;
    Logger::debug (req->getOutputFd())(" : (w) added").flush();
}

void  SelectMultiplexer::remove(IProxyRequest* req, IMultiplexer::mod_t mod)
{
    ProxyRequests::iterator it = proxyRequests.find(req->getOutputFd());

    if (it == proxyRequests.end())
        return;

    if (mod == IMultiplexer::READ)
    {
        if (req->getSocketFd() == -1) return;

        FD_CLR(req->getSocketFd(), &mReadfds);
        Logger::debug (req->getSocketFd())(" : (r) removed").flush();

        if (!FD_ISSET(req->getOutputFd(), &mWritefds)) {
          proxyRequests.erase(it);

          if (req->getSocketFd() == mMaxfd - 1)
              updateMaxFd();
        }

        return;
    }

    FD_CLR(req->getOutputFd(), &mWritefds);
    Logger::debug (req->getOutputFd())(" : (w) removed").flush();

    if (req->getSocketFd() == -1 || !FD_ISSET(req->getSocketFd(), &mReadfds)) {
      proxyRequests.erase(it);

      if (req->getOutputFd() == mMaxfd - 1)
          updateMaxFd();
    }
}

void  SelectMultiplexer::add(IProxyResponse* res, IMultiplexer::mod_t mod)
{
    proxyResponses[res->getSocketFd()] = res;

    if (mod == IMultiplexer::READ)
    {
        FD_SET(res->getInputFd(), &mReadfds);
        Logger::debug (res->getInputFd())(" : (r) added").flush();
        mMaxfd = mMaxfd > res->getInputFd() ? mMaxfd : res->getInputFd() + 1;
        return;
    }
    
    FD_SET(res->getSocketFd(), &mWritefds);
    Logger::debug (res->getSocketFd())(" : (w) added").flush();
    mMaxfd = mMaxfd > res->getSocketFd() ? mMaxfd : res->getSocketFd() + 1;
}

void  SelectMultiplexer::remove(IProxyResponse* res, IMultiplexer::mod_t mod)
{
    ProxyResponses::iterator it = proxyResponses.find(res->getSocketFd());

    if (it == proxyResponses.end()) {
        Logger::debug ("cgi res not)(nd\n").flush();
        return;
    }

    // proxyResponses.erase(it);
    if (mod == IMultiplexer::READ)
    {
        FD_CLR(res->getInputFd(), &mReadfds);
        Logger::debug (res->getInputFd())(" : (r) removed").flush();

        if (!FD_ISSET(res->getSocketFd(), &mWritefds)) {
          proxyResponses.erase(it);

          if (res->getInputFd() == mMaxfd - 1)
              updateMaxFd();
        }
        return;
    }

    FD_CLR(res->getSocketFd(), &mWritefds);
    Logger::debug (res->getSocketFd())(" : (w) removed").flush();

    if (!FD_ISSET(res->getInputFd(), &mReadfds)) {
        proxyResponses.erase(it);

      if (res->getSocketFd() == mMaxfd - 1)
          updateMaxFd();
    }
}

std::queue<IProxyRequest*>  SelectMultiplexer::getReadyForReadingProxyRequests() const
{
    // Logger::debug ("proxyRequests.size())( << proxyRequests.size() << std::endl).flush();
    std::queue<IProxyRequest *> ret;

    if (mReadyfdsCount == 0)
        return (ret);

    ProxyRequests::const_iterator it = proxyRequests.begin();

    while (it != proxyRequests.end()) {
        if (it->second->getSocketFd() != -1 &&
                FD_ISSET(it->second->getSocketFd(), &mReadfdsTmp))
            ret.push(it->second);
        ++it;
    }
    return (ret);
}

std::queue<IProxyRequest*>  SelectMultiplexer::getReadyForWritingProxyRequests() const
{
  std::queue<IProxyRequest *> ret;

  if (mReadyfdsCount == 0)
    return (ret);

  ProxyRequests::const_iterator it = proxyRequests.begin();

  while (it != proxyRequests.end()) {
    if (FD_ISSET(it->first, &mWritefdsTmp))
      ret.push(it->second);
    ++it;
  }
  return (ret);
}

std::queue<IProxyResponse*>  SelectMultiplexer::getReadyForReadingProxyResponses() const
{
//   Logger::debug ("proxyResponses.size())( << proxyResponses.size() << std::endl).flush();
  std::queue<IProxyResponse *> ret;

  if (mReadyfdsCount == 0)
    return (ret);

  ProxyResponses::const_iterator it = proxyResponses.begin();

  while (it != proxyResponses.end()) {
    if (FD_ISSET(it->second->getInputFd(), &mReadfdsTmp))
      ret.push(it->second);
    ++it;
  }
  return (ret);
}

std::queue<IProxyResponse*>  SelectMultiplexer::getReadyForWritingProxyResponses() const
{
  std::queue<IProxyResponse *> ret;

  if (mReadyfdsCount == 0 || proxyResponses.empty())
    return (ret);

  ProxyResponses::const_iterator it = proxyResponses.begin();

  while (it != proxyResponses.end()) {
    if (FD_ISSET(it->first, &mWritefdsTmp))
      ret.push(it->second);
    ++it;
  }
  return (ret);
}



/******************************************************************************
         Wait
******************************************************************************/

void SelectMultiplexer::wait(std::size_t timeout_us)
{
    mReadfdsTmp = mReadfds;
    mWritefdsTmp = mWritefds;

    /*
    {
        int*  r = reinterpret_cast<int*>(&mReadfds);
        int*  w = reinterpret_cast<int*>(&mWritefds);

        int idx, shift;
        int size_of_int = 8 * sizeof(int);

        for (int i = mMaxfd - 1; i >= 0; --i)
        {
        idx = i / size_of_int;
        shift = i % size_of_int;

        bool rb = (r[idx] >> shift) & 1;
        bool wb = (w[idx] >> shift) & 1;
        
        if (rb || wb)
            Logger::debug (i;
        if (rb)(          Logger::debug ('r').flush();
        if)().flush();
            Logger::debug ('w';
        if)( || wb).flush();
            Logger::debug (' ';
        }
        Logger)(bug (std::endl;)( ).flush();
    */

    struct timeval timeout;
    timeout.tv_sec = static_cast<long>(timeout_us / 1000000);
    timeout.tv_usec = static_cast<long>(timeout_us % 1000000);
  
    mReadyfdsCount = ::select(mMaxfd, &mReadfdsTmp, &mWritefdsTmp, NULL, &timeout);

    // Logger::debug (mReadyfdsCount << std::)().flush();
      
  // Logger::debug (mReadyfdsCount << ' ')(proxyRequests.size() << ' ' << proxyResponses.size() << std::endl).flush();
}

bool  SelectMultiplexer::ready() const
{
  return (mReadyfdsCount > 0);
}

void  SelectMultiplexer::updateMaxFd()
{
  int*  r = reinterpret_cast<int*>(&mReadfds);
  int*  w = reinterpret_cast<int*>(&mWritefds);

  int idx, shift;
  int size_of_int = 8 * sizeof(int);

  for (int i = mMaxfd - 1; i >= 0; --i)
  {
    idx = i / size_of_int;
    shift = i % size_of_int;
    if ((r[idx] >> shift) & 1 || (w[idx] >> shift) & 1)
    {
      mMaxfd = i + 1;
      return;
    }
  }
  mMaxfd = 0;
}
