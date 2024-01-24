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
#include <unistd.h>


#include "src/Interfaces/IClientSocket.hpp"
#include "src/Interfaces/IProxyResponse.hpp"

class CGIResponse : public IProxyResponse
{
    int                     mInputFd;
    const IClientSocket&    mSocket;
    std::string             mBuffer;
    // std::string             mReadingBuffer;
    bool                    mEof;


	CGIResponse( const CGIResponse& p);
	CGIResponse&	operator=( const CGIResponse& p );

    void    readAll();
    
public:
    CGIResponse(int fd, const IClientSocket& sock);
	~CGIResponse();

    int     getInputFd() const;
    int     getSocketFd() const;

    void    build();

    void    send();
    bool    done() const;

};
#endif
