/*	                     __          _
 *	   __  ___________ _/ /___ ___  (_)
 *	  / / / / ___/ __ `/ / __ `__ \/ /
 *	 / /_/ (__  ) /_/ / / / / / / / / 
 *	 \__, /____/\__,_/_/_/ /_/ /_/_/ 
 *	/____/	User: Youssef Salmi
 *			File: IServerSocket.hpp 
 */

#pragma once
#ifndef ISERVERSOCKET_HPP
#define ISERVERSOCKET_HPP

#include "IClientSocket.hpp"

class IServerSocket
{
public:
	virtual		~IServerSocket(){};
	virtual void        	bind(unsigned int aIP, unsigned int aPORT) = 0;
    virtual void        	listen() = 0;
    virtual IClientSocket*	accept() = 0;
};
#endif
