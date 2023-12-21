/*	                     __          _
 *	   __  ___________ _/ /___ ___  (_)
 *	  / / / / ___/ __ `/ / __ `__ \/ /
 *	 / /_/ (__  ) /_/ / / / / / / / / 
 *	 \__, /____/\__,_/_/_/ /_/ /_/_/ 
 *	/____/	User: Youssef Salmi
 *			File: ProxiedResponse.hpp 
 */

#pragma once
#ifndef PROXIEDRESPONSE_HPP
#define PROXIEDRESPONSE_HPP

#include <iostream>

#include "IProxiedResponse.hpp"

class ProxiedResponse : public IProxiedResponse
{
public:
	ProxiedResponse();
	ProxiedResponse( const ProxiedResponse& p);
	~ProxiedResponse();

	ProxiedResponse&	operator=( const ProxiedResponse& p );

};
#endif
