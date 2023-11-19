/*	                     __          _
 *	   __  ___________ _/ /___ ___  (_)
 *	  / / / / ___/ __ `/ / __ `__ \/ /
 *	 / /_/ (__  ) /_/ / / / / / / / / 
 *	 \__, /____/\__,_/_/_/ /_/ /_/_/ 
 *	/____/	User: Youssef Salmi
 *			File: IResponse.hpp 
 */

#pragma once
#ifndef IRESPONSE_HPP
#define IRESPONSE_HPP

#include <string>

class IResponse
{
public:
    virtual ~IResponse(){};
    virtual void        activate() = 0;
    virtual void        send() = 0;
    virtual IResponse&  setStatusCode(int aStatusCode) = 0;
    virtual IResponse&  setHeader(const std::string& aKey, const std::string& aValue ) = 0;
    virtual bool        isComplete() = 0;
};
#endif