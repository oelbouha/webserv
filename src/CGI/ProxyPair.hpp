/*	                     __          _
 *	   __  ___________ _/ /___ ___  (_)
 *	  / / / / ___/ __ `/ / __ `__ \/ /
 *	 / /_/ (__  ) /_/ / / / / / / / / 
 *	 \__, /____/\__,_/_/_/ /_/ /_/_/ 
 *	/____/	User: Youssef Salmi
 *			File: ProxyPair.hpp 
 */

#pragma once
#ifndef PROXYPAIR_HPP
#define PROXYPAIR_HPP

#include <cstddef>
#include <sys/wait.h>

#include "src/Interfaces/IProxyRequest.hpp"
#include "src/Interfaces/IProxyResponse.hpp"

class ProxyPair
{
    int                 mPID;
    IProxyRequest*      mRequest;
    IProxyResponse*     mResponse;

	ProxyPair&	operator=( const ProxyPair& p );
	ProxyPair( const ProxyPair& p );

public:
    ProxyPair(int pid, IProxyRequest* req, IProxyResponse* res);
	~ProxyPair();

    IProxyRequest*   request();
    IProxyResponse*  response();

    void    destroyRequest();
};
#endif
