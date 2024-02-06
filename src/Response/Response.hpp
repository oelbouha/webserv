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
#include <fstream>
#include <cstddef>
#include <string>
#include <map>

#include <unistd.h>
#include <fcntl.h>

#include "IClientSocket.hpp"
#include "IResponse.hpp"

#include "src/Server/MimeTypes.hpp"
#include "src/Socket/SocketException.hpp"
#include "src/Response/ResponseException.hpp"

#include "Utils.hpp"
#include "Types.hpp"

class Response : public IResponse
{
    const IClientSocket &mSocket;
    unsigned int mStatusCode;
    string_string_map mHeaders;

    std::string mBody;
    int mFile;

    std::string mRawResponse;

    size_t mCursor;
    bool isComplete;

    Response(const Response &aResponse);
    Response &operator=(const Response &aResponse);

public:
    Response(const IClientSocket &aSocket);
    ~Response();

    virtual int getSocketFd() const;

    Response &setStatusCode(unsigned int aStatusCode);
    Response &setHeader(const std::string &aHeader, const std::string &aValue);
    Response &setBody(const std::string &aBody);
    Response &setBodyFile(const std::string &aFileName);

    Response &build();

    void send();
    bool done() const;

    void dump();

    static const uint_string_map StatusCodes;
    static uint_string_map initStatusCodes();
};
#endif
