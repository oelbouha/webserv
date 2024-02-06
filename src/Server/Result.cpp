/*	                     __          _
 *	   __  ___________ _/ /___ ___  (_)
 *	  / / / / ___/ __ `/ / __ `__ \/ /
 *	 / /_/ (__  ) /_/ / / / / / / / / 
 *	 \__, /____/\__,_/_/_/ /_/ /_/_/ 
 *	/____/	User: Youssef Salmi
 *			File: Result.cpp 
 */

#include "Result.hpp"

Result::Result(IResponse* res) :
    type(Result::RESPONSE)
{
    mResponse = res;
}

Result::Result(ProxyPair pair):
    type(Result::PROXY_PAIR)
{
    mProxyPair = pair;
}

Result::Result(Upload *upload):
    type(Result::UPLOAD)
{
    mUpload = upload;
}

Result::Result(const Result& res):
    type(res.type)
{
    mResponse = res.mResponse;
    mProxyPair = res.mProxyPair;
}

Result::~Result()
{}

IResponse*  Result::response()
{
    return (mResponse);
}

Upload*  Result::upload()
{
    return (mUpload);
}

ProxyPair   Result::proxyPair()
{
    return (mProxyPair);
}
