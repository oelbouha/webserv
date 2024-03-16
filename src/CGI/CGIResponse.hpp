/*	                     __          _
 *	   __  ___________ _/ /___ ___  (_)
 *	  / / / / ___/ __ `/ / __ `__ \/ /
 *	 / /_/ (__  ) /_/ / / / / / / / / 
 *	 \__, /____/\__,_/_/_/ /_/ /_/_/ 
 *	/____/	User: Youssef Salmi
 *			File: CGIResponse.hpp 
 */

#pragma once
#ifndef CGIRESPONSE_HPP
#define CGIRESPONSE_HPP

#include <iostream>
#include <string>
#include <sstream>
#include <map>

#include <unistd.h>
#include <errno.h>

#include "Logger.hpp"


#include "src/Interfaces/IClientSocket.hpp"
#include "src/Interfaces/IProxyResponse.hpp"
#include "src/Interfaces/IClient.hpp"
#include "src/CGI/IResponseWriter.hpp"
#include "src/CGI/DefaultWriter.hpp"
#include "src/CGI/ChunkedWriter.hpp"
#include "src/Socket/SocketException.hpp"

#include "Utils.hpp"
#include "Types.hpp"

class CGIResponse : public IProxyResponse
{
    int                     mInputFd;
    const IClientSocket&    mSocket;
    std::string             mHeader;
    bool                    mEof;
    bool                    mHeaderComplete;
    int                     mSent;
    string_string_map       mResponseHeaders;

    IResponseWriter*        mWriter;

public:
    IClient*                client;
    IRequest*               request;

private:
	CGIResponse( const CGIResponse& p);
	CGIResponse&	operator=( const CGIResponse& p );

    void    parseHeader();
    
public:
    CGIResponse(int fd, IRequest* request);
	~CGIResponse();

    int     getInputFd() const;
    int     getSocketFd() const;

    void    build();

    bool    isLocalRedirection() const;
    std::string getRedirectionLocation() const;

    void    read();
    void    send();
    bool    sent() const;
    bool    done() const;
    bool    error() const;

};
#endif
