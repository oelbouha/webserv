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
#include <string>

#include <unistd.h>

#include "src/Interfaces/IRequest.hpp"

#include "DescriptorProxyRequest.hpp"
#include "BufferProxyRequest.hpp"
#include "ProxyPair.hpp"
#include "IProxyHandler.hpp"
#include "CGIResponse.hpp"

class CGIHandler
{
private:
	static std::vector<std::string>	compileEnv(IRequest& req);
	CGIHandler();
public:

    static ProxyPair   handle(IRequest* request, const std::string& path);
};
#endif
