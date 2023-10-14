/*	                     __          _
 *	   __  ___________ _/ /___ ___  (_)
 *	  / / / / ___/ __ `/ / __ `__ \/ /
 *	 / /_/ (__  ) /_/ / / / / / / / / 
 *	 \__, /____/\__,_/_/_/ /_/ /_/_/ 
 *	/____/	User: Youssef Salmi
 *			File: IServer.hpp 
 */

#pragma once
#ifndef ISERVER_HPP
#define ISERVER_HPP

#include "IRequest.hpp"
#include "IResponse.hpp"

class IServer
{
	public:
		//virtual ;
		virtual IResponse	handle( const IRequest& req ) = 0;
		virtual	~IServer(){};

};
#endif