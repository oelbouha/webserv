/*	                     __          _
 *	   __  ___________ _/ /___ ___  (_)
 *	  / / / / ___/ __ `/ / __ `__ \/ /
 *	 / /_/ (__  ) /_/ / / / / / / / / 
 *	 \__, /____/\__,_/_/_/ /_/ /_/_/ 
 *	/____/	User: Youssef Salmi
 *			File: IResponse.hpp 
 */

#pragma once
#ifndef IRESPONSE_HPP
#define IRESPONSE_HPP

class IResponse
{
public:
    virtual ~IResponse(){};
    virtual send() = 0;
    virtual setStatusCode() = 0;

};
#endif