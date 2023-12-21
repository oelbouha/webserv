/*	                     __          _
 *	   __  ___________ _/ /___ ___  (_)
 *	  / / / / ___/ __ `/ / __ `__ \/ /
 *	 / /_/ (__  ) /_/ / / / / / / / /
 *	 \__, /____/\__,_/_/_/ /_/ /_/_/
 *	/____/	User: Youssef Salmi
 *			File: ServerSocket.cpp
 */

#include "ServerSocket.hpp"
#include <iostream>

int ServerSocket::sBackLog = 5;

ServerSocket::ServerSocket() : mIP(utils::ip(0, 0, 0, 0)), mPort(8080) {
  if ((mID = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    throw SocketException("Can't create socket");
}

ServerSocket::ServerSocket(unsigned int aIP, unsigned short aPort)
    : mIP(aIP), mPort(aPort) {
  if ((mID = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    throw SocketException("Can't create socket");
}

ServerSocket::ServerSocket(const ServerSocket &s)
    : mID(s.mID), mIP(s.mIP), mPort(s.mPort) {}

ServerSocket::~ServerSocket() {}

ServerSocket &ServerSocket::operator=(const ServerSocket &aServerSocket) {
  if (this != &aServerSocket) {
  }
  return (*this);
}

int ServerSocket::getID() const { return mID; }

void ServerSocket::bind() {
  struct sockaddr_in addr;
  std::size_t addr_len = sizeof(addr);

  int opt = 1;
  if (::setsockopt(mID, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
    throw SocketException(std::string("Can't reuse addr/port ") +
                          strerror(errno));
  if (::setsockopt(mID, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt)) < 0)
    throw SocketException(std::string("Can't reuse addr/port ") +
                          strerror(errno));

  bzero(&addr, addr_len);
  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = htonl(mIP);
  addr.sin_port = htons(mPort);

  if (::bind(mID, (struct sockaddr *)&addr, addr_len) < 0) {
    throw SocketException("Can't bind socket to " + std::to_string(mIP) + ":" +
                          std::to_string(mPort) + " " + strerror(errno));
  }
}

void ServerSocket::listen() {
  if (::listen(mID, ServerSocket::sBackLog) < 0)
    throw SocketException("Socket is not listenning");
}

IClientSocket *ServerSocket::accept() const {
  struct sockaddr_in addr;
  unsigned int  addr_len = sizeof(addr);

  int id = ::accept(mID, (struct sockaddr *)&addr, (socklen_t *)&addr_len);

  std::cout << "accepted: " << utils::ip(ntohl(addr.sin_addr.s_addr))
            << "::" << ntohs(addr.sin_port) << " on " << utils::ip(ntohl(mIP))
            << std::endl
            << std::flush;

  return (new ClientSocket(id));
}

void ServerSocket::close() { ::close(mID); }

void ServerSocket::setNonBlocking() {
  if (fcntl(mID, F_SETFL, O_NONBLOCK, O_CLOEXEC) < 0)
    throw SocketException("Can't switch socket to non-blocking mode");
}

int ServerSocket::getPort() const { return (mPort); }

ip_t ServerSocket::getIP() const { return (mIP); }
