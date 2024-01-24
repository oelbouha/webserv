/*	                     __          _
 *	   __  ___________ _/ /___ ___  (_)
 *	  / / / / ___/ __ `/ / __ `__ \/ /
 *	 / /_/ (__  ) /_/ / / / / / / / / 
 *	 \__, /____/\__,_/_/_/ /_/ /_/_/ 
 *	/____/	User: Youssef Salmi
 *			File: ResponseWrapper.hpp 
 */

#pragma once
#ifndef RESPONSEWRAPPER_HPP
#define RESPONSEWRAPPER_HPP

#include "src/Response/Response.hpp"
//#include "src/Response/SimpleResponse.hpp"
#include "src/Response/ProxiedResponse.hpp"

typedef Response    SimpleResponse;

class ResponseWrapper
{
    SimpleResponse*     mSimpleResponse;
    ProxiedResponse*    mProxiedResponse;

public:
    enum ResponseType
    {
        SIMPLE_RESPONSE,
        PROXIED_RESPONSE
    };

public:

    ResponseWrapper(SimpleResponse* res);
    ResponseWrapper(ProxiedResponse* res);
	ResponseWrapper( const ResponseWrapper& r );
	~ResponseWrapper();

	ResponseWrapper&	operator=( const ResponseWrapper& r );

    ResponseType    getType() const;

    SimpleResponse*     getSimpleResponse() const;
    ProxiedResponse*    getProxiedResponse() const;
};
#endif
