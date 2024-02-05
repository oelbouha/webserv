/*	                     __          _
 *	   __  ___________ _/ /___ ___  (_)
 *	  / / / / ___/ __ `/ / __ `__ \/ /
 *	 / /_/ (__  ) /_/ / / / / / / / / 
 *	 \__, /____/\__,_/_/_/ /_/ /_/_/ 
 *	/____/	User: Youssef Salmi
 *			File: IRequest.hpp 
 */

#pragma once
#ifndef IREQUEST_HPP
#define IREQUEST_HPP

#include <string>

#include "Types.hpp"

#include "src/Interfaces/IClientSocket.hpp"

class IRequest
{
public:
    virtual ~IRequest(){};
    virtual const IClientSocket&    getSocket() const = 0;
    virtual int                     getSocketFd() const = 0;
    virtual const std::string&      getMethod() const = 0;
    virtual const std::string&      getURI() const = 0;
    virtual const std::string&      getHttpVersion() const = 0;
    virtual const std::string&      getHeader( const std::string& key ) const = 0;
    virtual const std::string&      getQuery() const = 0;
    virtual size_t                  getContentLength() const = 0;

    virtual unsigned int            getIncomingIP() const = 0;
    virtual unsigned int            getIncomingPort() const = 0;

    virtual void                build() = 0;
    
    virtual std::string         read() = 0;
    virtual bool                done() const = 0;
    
};
#endif
