/*	                     __          _
 *	   __  ___________ _/ /___ ___  (_)
 *	  / / / / ___/ __ `/ / __ `__ \/ /
 *	 / /_/ (__  ) /_/ / / / / / / / / 
 *	 \__, /____/\__,_/_/_/ /_/ /_/_/ 
 *	/____/	User: Youssef Salmi
 *			File: ProxyPair.cpp 
 */

#include "ProxyPair.hpp"

ProxyPair::ProxyPair():
    request(NULL),
    response(NULL),
    mPID(-1)
{}

ProxyPair::ProxyPair(int pid, IProxyRequest* req, IProxyResponse* res):
    request(req),
    response(res),
    mPID(pid)
{}

ProxyPair::ProxyPair( const ProxyPair& p ) :
    request(p.request),
    response(p.response),
    mPID(p.mPID)
{}

ProxyPair::~ProxyPair()
{}

ProxyPair&	ProxyPair::operator=( const ProxyPair& p )
{
    if (this != &p)
    {
        request = p.request;
        response = p.response;
        mPID = p.mPID;
    }
	return (*this);
}

void    ProxyPair::setChildFree()
{
    if (mPID == -1) return;
    
    int ret = ::waitpid(mPID, NULL, WNOHANG);
    if (ret == -1)
        std::perror("proxy pair");
    if (ret == 0)
    {
        kill(mPID, SIGKILL);
        ::waitpid(mPID, NULL, 0);
    }
    mPID = -1;
}

