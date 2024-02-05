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
// #include "src/Route/Upload.hpp"

class Result
{
public:
    Result(IResponse* res);
    Result(ProxyPair pair);
    // Result(ProxyPair* pair); //for upload
    
    Result(const Result& result);
    ~Result();

    IResponse*  response();
    ProxyPair   proxyPair();
    // Upload*     upload();

    enum    Type
    {
        RESPONSE,
        PROXY_PAIR,
        UPLOAD
    };
    
    const Type   type;

private:
    IResponse*  mResponse;
    ProxyPair   mProxyPair;
    // Upload*     mUpload;
};
#endif
