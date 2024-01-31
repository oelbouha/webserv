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

#include <iostream>

#include <cstddef>
#include <sys/wait.h>
#include <signal.h>

#include "src/Interfaces/IClient.hpp"
#include "src/Interfaces/IProxyRequest.hpp"
#include "src/Interfaces/IProxyResponse.hpp"

struct ProxyPair
{
    IProxyRequest*      request;
    IProxyResponse*     response;

    ProxyPair();
    ProxyPair(int pid, IProxyRequest* req, IProxyResponse* res);
	ProxyPair( const ProxyPair& p );
	~ProxyPair();

	ProxyPair&	operator=( const ProxyPair& p );

    void    setChildFree();

private:
    int                 mPID;

};
#endif
