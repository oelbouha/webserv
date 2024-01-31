/*	                     __          _
 *	   __  ___________ _/ /___ ___  (_)
 *	  / / / / ___/ __ `/ / __ `__ \/ /
 *	 / /_/ (__  ) /_/ / / / / / / / / 
 *	 \__, /____/\__,_/_/_/ /_/ /_/_/ 
 *	/____/	User: Youssef Salmi
 *			File: Result.hpp 
 */

#pragma once
#ifndef RESULT_HPP
#define RESULT_HPP

#include "src/CGI/ProxyPair.hpp"
#include "src/Interfaces/IResponse.hpp"

class Result
{
public:
    Result(IResponse* res);
    Result(ProxyPair* pair);
    Result(const Result& result);
    ~Result();

    IResponse*  response();
    ProxyPair*  proxyPair();

    enum    Type
    {
        RESPONSE,
        PROXY_PAIR
    };
    
    const Type   type;

private:
    union
    {
        IResponse*  mResponse;
        ProxyPair   mProxyPair;
    };
};
#endif
