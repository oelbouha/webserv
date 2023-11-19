/*	                     __          _
 *	   __  ___________ _/ /___ ___  (_)
 *	  / / / / ___/ __ `/ / __ `__ \/ /
 *	 / /_/ (__  ) /_/ / / / / / / / / 
 *	 \__, /____/\__,_/_/_/ /_/ /_/_/ 
 *	/____/	User: Youssef Salmi
 *			File: IClientSocket.hpp 
 */

#pragma once
#ifndef ICLIENTSOCKET_HPP
#define ICLIENTSOCKET_HPP

#include <string>

class IClientSocket
{
public:
	virtual		~IClientSocket(){};
	// write buffer to socket and return how many bytes were written
    virtual int         write(const char* buffer, int size) = 0;

    // one read call - returns empty string if no data is found
    virtual std::string read(unsigned int aMaxSize) = 0;

    // read all availabe data on socket
    virtual std::string readAll() = 0;

    virtual void        dump() = 0;
};
#endif
