/*	                     __          _
 *	   __  ___________ _/ /___ ___  (_)
 *	  / / / / ___/ __ `/ / __ `__ \/ /
 *	 / /_/ (__  ) /_/ / / / / / / / /
 *	 \__, /____/\__,_/_/_/ /_/ /_/_/
 *	/____/	User: Youssef Salmi
 *			File: Response.hpp
 */

#pragma once
#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include <iostream>
#include <map>
#include <fstream>

#include <unistd.h>
#include <fcntl.h>

#include "src/Server/MimeTypes.hpp"
#include "IClientSocket.hpp"
#include "IResponse.hpp"
#include "Utils.hpp"

class Response : public IResponse {
  IClientSocket&                        mSocket;
  unsigned int                          mStatusCode;
  std::map<std::string, std::string>    mHeaders;

  std::string   mBody;
  int           mFile;

  std::string mRawResponse;

  size_t    mCursor;
  bool      isComplete;

public:
  Response(IClientSocket &aSocket);
  Response(const Response &aResponse);
  ~Response();

  Response &operator=(const Response &aResponse);

  virtual int getID() const;

  Response &setStatusCode(unsigned int aStatusCode);
  Response &setHeader(const std::string &aHeader, const std::string &aValue);
  Response &setBody(const std::string &aBody);
  Response &setBodyFile(const std::string &aFileName);

  Response &build();

  Response &startSending();
  void send();
  bool isSendingComplete();

  void dump();
  void dumpHeader();

  static const std::map<unsigned int, std::string> sStatusCodes;
  static std::map<unsigned int, std::string> initStatusCodes();
};
#endif
