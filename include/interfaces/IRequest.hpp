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

class IRequest
{
public:
    virtual ~IRequest(){};
    virtual method_t            getMethod() const = 0;
    virtual const std::string&  getURI() const = 0;
    virtual const std::string&  getHttpVersion() const = 0;
    virtual const std::string&  getHeader( const std::string& key ) const = 0;
    virtual const std::string&  getQuery() const = 0;
};
#endif