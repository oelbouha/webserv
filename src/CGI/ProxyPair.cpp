/*	                     __          _
 *	   __  ___________ _/ /___ ___  (_)
 *	  / / / / ___/ __ `/ / __ `__ \/ /
 *	 / /_/ (__  ) /_/ / / / / / / / / 
 *	 \__, /____/\__,_/_/_/ /_/ /_/_/ 
 *	/____/	User: Youssef Salmi
 *			File: ProxyPair.cpp 
 */

#include "ProxyPair.hpp"

ProxyPair::ProxyPair(int pid, IProxyRequest* req, IProxyResponse* res):
    mPID(pid),
    mRequest(req),
    mResponse(res)
{}

ProxyPair::ProxyPair( const ProxyPair& p ) :
    mRequest(p.mRequest),
    mResponse(p.mResponse)
{}

ProxyPair::~ProxyPair()
{
    delete mRequest;
    delete mResponse;
    ::waitpid(mPID, NULL, 0);
}

ProxyPair&	ProxyPair::operator=( const ProxyPair& p )
{
    if (this != &p)
    {
        mRequest = p.mRequest;
        mResponse = p.mResponse;
    }
	return (*this);
}

IProxyRequest*   ProxyPair::request()
{
    return (mRequest);
}

IProxyResponse*      ProxyPair::response()
{
    return (mResponse);
}

void            ProxyPair::destroyRequest()
{
    delete mRequest;
    mRequest = NULL;
}
