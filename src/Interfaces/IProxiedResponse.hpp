/*	                     __          _
 *	   __  ___________ _/ /___ ___  (_)
 *	  / / / / ___/ __ `/ / __ `__ \/ /
 *	 / /_/ (__  ) /_/ / / / / / / / / 
 *	 \__, /____/\__,_/_/_/ /_/ /_/_/ 
 *	/____/	User: Youssef Salmi
 *			File: IProxiedResponse.hpp 
 */

#pragma once
#ifndef IPROXIEDRESPONSE_HPP
#define IPROXIEDRESPONSE_HPP

class IProxiedResponse
{
public:
	virtual		~IProxiedResponse(){};
    void        forward();
    void        send();
};
#endif
