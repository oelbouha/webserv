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

#define CGI_BUFFER_SIZE 4096
#define CGI_SEND_TIMEOUT 10
#define CGI_READ_TIMEOUT 5

#include <iostream>
#include <string>
#include <sstream>
#include <map>
#include <ctime>

#include <unistd.h>
#include <errno.h>

#include "src/Interfaces/IClientSocket.hpp"
#include "src/Interfaces/IProxyResponse.hpp"
#include "src/Interfaces/IClient.hpp"

#include "src/Server/ErrorPages.hpp"
#include "src/CGI/IResponseWriter.hpp"
#include "src/CGI/DefaultWriter.hpp"
#include "src/CGI/ChunkedWriter.hpp"
#include "src/Socket/SocketException.hpp"

#include "Logger.hpp"
#include "Utils.hpp"
#include "Types.hpp"

class CGIResponse : public IProxyResponse
{
    int                     mInputFd;
    const IClientSocket&    mSocket;
    std::string             mHeader;
    std::string             mBody;
    bool                    mEof;
    bool                    mHeaderComplete;
    bool                    mError;
    int                     mSent;
    string_string_map       mResponseHeaders;
    std::string             mAdditionalHeader;
    IResponseWriter*        mWriter;
    const ErrorPages*       mErrorPages;
    time_t                  mLastReadTimeStamp;
    time_t                  mLastSendTimeStamp;

public:
    IClient*                client;
    IRequest*               request;

private:
	CGIResponse( const CGIResponse& p);
	CGIResponse&	operator=( const CGIResponse& p );

    void    parseHeader();
    void    build();
    void    setWriter();
    void    extractHeader();
public:
    CGIResponse(int fd, IRequest* request);
	~CGIResponse();

    int     getInputFd() const;
    int     getSocketFd() const;
    void    setErrorPages( const ErrorPages* error_pages );
    void    setHeader(const std::string& header, const std::string& value);

    bool        isLocalRedirection() const;
    std::string getRedirectionLocation() const;
    IResponse*  buildErrorPage(int code) const;
    bool        hasTimedOut() const;

    void    read();
    void    send();
    bool    sent() const;
    bool    done() const;
    bool    error();

};
#endif
