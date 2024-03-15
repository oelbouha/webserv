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

#include "src/Interfaces/IClientSocket.hpp"
#include "src/Interfaces/IRequest.hpp"

#include "src/Request/DefaultReader.hpp"
#include "src/Request/ChunkedReader.hpp"
#include "src/Request/RequestException.hpp"

#include "src/Socket/SocketException.hpp"

#include "Utils.hpp"
#include "Types.hpp"
#include "Logger.hpp"

class Request : public IRequest
{
    IClientSocket&  mSocket;
    IRequestReader* mReader;

public:
    const int             incomingIP;
    const int             incomingPort;

private:
    std::string         mMethod;
    std::string         mUri;
    std::string         mQuery;
    std::string         mHttpVersion;
    string_string_map   mHeaders;
    std::string         mNoHeader;

public:
    Request(IClientSocket &mSocket, int aIncomingIP, int aIncomingPort);
    Request(const Request &aRequest);
    ~Request();

    const IClientSocket&    getSocket() const;
    int                     getSocketFd() const;

    const std::string&  getMethod() const;
    const std::string&  getURI() const;
    const std::string&  getHttpVersion() const;
    const std::string&  getQuery() const;
    const std::string&  getHeader(const std::string &aKey) const;
    void                setHeader(const std::string& key, const std::string& val);
    size_t              getContentLength() const;

    void                build();
    std::string         read();
    bool                done() const;
    virtual void        dump( bool colors = true ) const;


private:
    Request &operator=(const Request &aRequest);

    void parse();
    void parseRequestLine(const std::string &aRequestLine);
    void parseHeaderProperty(const std::string &aHeaderLine);
    void setMethod(const std::string &aMethod);
};
#endif
