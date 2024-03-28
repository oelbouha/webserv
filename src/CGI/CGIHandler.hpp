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

#include "ProxyPair.hpp"
#include "CGIRequest.hpp"
#include "CGIResponse.hpp"

#include "src/Request/Request.hpp"

class CGIHandler
{
private:
	static std::vector<std::string>	compileEnv(Request& req);
	CGIHandler();
public:

    static ProxyPair   handle(Request* request, const std::string& path);
};
#endif
