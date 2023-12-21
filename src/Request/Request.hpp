/*	                     __          _
 *	   __  ___________ _/ /___ ___  (_)
 *	  / / / / ___/ __ `/ / __ `__ \/ /
 *	 / /_/ (__  ) /_/ / / / / / / / /
 *	 \__, /____/\__,_/_/_/ /_/ /_/_/
 *	/____/	User: Youssef Salmi
 *			File: Request.hpp
 */

#pragma once
#ifndef REQUEST_HPP
#define REQUEST_HPP

#include <iomanip>
#include <iostream>
#include <map>
#include <sstream>
#include <string>

#include "IClientSocket.hpp"
#include "IRequest.hpp"
#include "RequestException.hpp"
#include "Utils.hpp"

typedef std::map<std::string, std::string> RHeaders;

class Request : public IRequest {

  IClientSocket &mSocket;

  method_t mMethod;
  std::string mUri;
  std::string mQuery;
  std::string mHttpVersion;
  RHeaders mHeaders;

  int mIncommingIP;
  int mIncommingPort;

public:
  Request(IClientSocket &mSocket, int aIncomingIP, int aIncomingPort);
  Request(const Request &aRequest);
  ~Request();

public:
  Request &operator=(const Request &aRequest);

public:
  IClientSocket&    getSocket() const;
  int               getIncomingIP() const;
  int               getIncomingPort() const;
  method_t          getMethod() const;
  const std::string &getURI() const;
  const std::string &getHttpVersion() const;
  const std::string &getQuery() const;
  const std::string &getHeader(const std::string &aKey) const;

public:
  void build();

public:
  void dump(bool colors = true) const;

private:
  void parse();
  void parseRequestLine(const std::string &aRequestLine);
  void parseHeaderProperty(const std::string &aHeaderLine);
  void setMethod(const std::string &aMethod);
};
#endif
