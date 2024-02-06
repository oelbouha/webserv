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

#include "DefaultRequestReader.hpp"
#include "ChunkedRequestReader.hpp"

#include "src/Socket/SocketException.hpp"

#include "Types.hpp"

class Request : public IRequest
{
    IClientSocket&  mSocket;
    IRequestReader* mReader;

    int             mIncommingIP;
    int             mIncommingPort;

    std::string         mMethod;
    std::string         mUri;
    std::string         mQuery;
    std::string         mHttpVersion;
    string_string_map   mHeaders;

    std::string         mNoHeader;

public:
    Request(IClientSocket &mSocket, int aIncomingIP, int aIncomingPort);
    ~Request();

    const IClientSocket&    getSocket() const;
    int                     getSocketFd() const;

    unsigned int        getIncomingIP() const;
    unsigned int        getIncomingPort() const;
    const std::string&  getMethod() const;
    const std::string&  getURI() const;
    const std::string&  getHttpVersion() const;
    const std::string&  getQuery() const;
    const std::string&  getHeader(const std::string &aKey) const;
    size_t              getContentLength() const;

    void                build();
    std::string         read();
    bool                done() const;
    void                dump( bool colors = true ) const;

    Request(const Request &aRequest);
private:
    Request &operator=(const Request &aRequest);

    void parse();
    void parseRequestLine(const std::string &aRequestLine);
    void parseHeaderProperty(const std::string &aHeaderLine);
    void setMethod(const std::string &aMethod);
};
#endif
