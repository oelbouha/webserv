/*	                     __          _
 *	   __  ___________ _/ /___ ___  (_)
 *	  / / / / ___/ __ `/ / __ `__ \/ /
 *	 / /_/ (__  ) /_/ / / / / / / / / 
 *	 \__, /____/\__,_/_/_/ /_/ /_/_/ 
 *	/____/	User: Youssef Salmi
 *			File: CGIHandler.hpp 
 */

#pragma once
#ifndef CGIHANDLER_HPP
#define CGIHANDLER_HPP

#include <vector>

#include "DescriptorProxyRequest.hpp"
#include "ProxyPair.hpp"
#include <string>
#include <unistd.h>

#include "IProxyHandler.hpp"
#include "src/Interfaces/IRequest.hpp"

#include "ProxyPair.hpp"
#include "DescriptorProxyRequest.hpp"
#include "CGIResponse.hpp"

class CGIHandler
{
private:
	std::vector<std::string>	mArgs, mEnv;
	void	compileEnv(IRequest& req);
public:
	CGIHandler();
	CGIHandler( const CGIHandler& c);
	~CGIHandler();

	CGIHandler&	operator=( const CGIHandler& c );

    virtual ProxyPair   handle(IRequest* request);
};
#endif
