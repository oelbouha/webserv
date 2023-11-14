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

class IRequest
{
public:
    virtual ~IRequest(){};
    virtual const std::string& getMethod() = 0;
    virtual const std::string& getUri() = 0;
    virtual const std::string& getHttpVersion() = 0;
    virtual const std::string& getHost() = 0;
    virtual const std::string& getHeader( const std::string& key ) = 0;
    virtual const std::string& getQuery() = 0;
    virtual const std::string& getBody() = 0;
};
#endif