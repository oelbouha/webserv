/*	                     __          _
 *	   __  ___________ _/ /___ ___  (_)
 *	  / / / / ___/ __ `/ / __ `__ \/ /
 *	 / /_/ (__  ) /_/ / / / / / / / / 
 *	 \__, /____/\__,_/_/_/ /_/ /_/_/ 
 *	/____/	User: Youssef Salmi
 *			File: ResponseWrapper.cpp 
 */

#include "ResponseWrapper.hpp"
#include "ProxiedResponse.hpp"

ResponseWrapper::ResponseWrapper(SimpleResponse* res):
    mSimpleResponse(res), mProxiedResponse(NULL)
{}

ResponseWrapper::ResponseWrapper(ProxiedResponse* res):
    mSimpleResponse(NULL), mProxiedResponse(res)
{}

ResponseWrapper::ResponseWrapper( const ResponseWrapper& r )
{(void)r;}

ResponseWrapper::~ResponseWrapper()
{}

ResponseWrapper&	ResponseWrapper::operator=( const ResponseWrapper& r )
{
    (void)r;
	return (*this);
}

ResponseWrapper::ResponseType   ResponseWrapper::getType() const
{
    if (mSimpleResponse)
        return ResponseWrapper::SIMPLE_RESPONSE;
    return ResponseWrapper::PROXIED_RESPONSE;
}

SimpleResponse* ResponseWrapper::getSimpleResponse() const
{
    return (mSimpleResponse);
}

ProxiedResponse*    ResponseWrapper::getProxiedResponse() const
{
    return (mProxiedResponse);
}
