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


#include "src/Interfaces/IClientSocket.hpp"
#include "src/Interfaces/IProxyResponse.hpp"
#include "src/Socket/SocketException.hpp"

#include "Utils.hpp"
#include "Types.hpp"

class CGIResponse : public IProxyResponse
{
    int                     mInputFd;
    const IClientSocket&    mSocket;
    std::string             mHeader;
    std::string             mBuffer;
    bool                    mEof;
    int                     mFile;
    bool                    mHeaderComplete;
    int                     mSent;
    string_string_map       mResponseHeaders;


	CGIResponse( const CGIResponse& p);
	CGIResponse&	operator=( const CGIResponse& p );

    void    parseHeader();
    
public:
    CGIResponse(int fd, const IClientSocket& sock);
	~CGIResponse();

    int     getInputFd() const;
    int     getSocketFd() const;

    void    build();

    void    read();
    void    send();
    bool    sent() const;
    bool    done() const;
    bool    error() const;

};
#endif
