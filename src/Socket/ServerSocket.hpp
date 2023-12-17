/*	                     __          _
 *	   __  ___________ _/ /___ ___  (_)
 *	  / / / / ___/ __ `/ / __ `__ \/ /
 *	 / /_/ (__  ) /_/ / / / / / / / /
 *	 \__, /____/\__,_/_/_/ /_/ /_/_/
 *	/____/	User: Youssef Salmi
 *			File: ServerSocket.hpp
 */

#pragma once
#ifndef SERVERSOCKET_HPP
#define SERVERSOCKET_HPP

#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <iostream>
#include <netinet/in.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include "IServerSocket.hpp"
#include "SocketException.hpp"

#include "ClientSocket.hpp"

#include "Utils.hpp"

class ServerSocket : public IServerSocket {
  int mID;
  unsigned int mIP;
  unsigned short mPort;

  static int sBackLog;

public:
  ServerSocket();
  ServerSocket(unsigned int aIP, unsigned short aPort);
  ServerSocket(const ServerSocket &s);
  ~ServerSocket();

  ServerSocket &operator=(const ServerSocket &s);

  virtual void bind();
  virtual void listen();
  virtual IClientSocket *accept() const;
  virtual void close();
  virtual void setNonBlocking();

  virtual int getPort() const;
  virtual ip_t getIP() const;
  virtual int getID() const;
};
#endif
