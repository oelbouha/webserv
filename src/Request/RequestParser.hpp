/*	                     __          _
 *	   __  ___________ _/ /___ ___  (_)
 *	  / / / / ___/ __ `/ / __ `__ \/ /
 *	 / /_/ (__  ) /_/ / / / / / / / / 
 *	 \__, /____/\__,_/_/_/ /_/ /_/_/ 
 *	/____/	User: Youssef Salmi
 *			File: RequestParser.hpp 
 */

#pragma once
#ifndef REQUESTPARSER_HPP
#define REQUESTPARSER_HPP

#include <iostream>


#include "IClientSocket.hpp"
#include "SocketException.hpp"
#include "Request.hpp"

class RequestParser
{
	IClientSocket&	mClientSocket;
	Request			mRequest;

	RequestParser&	operator=( const RequestParser& aRequestParser );

public:
	RequestParser(IClientSocket& aClientSocket);
	RequestParser( const RequestParser& aRequestParser );
	~RequestParser();


	void			parse();
	const Request&	getRequest() const;

};
#endif
