/*	                     __          _
 *	   __  ___________ _/ /___ ___  (_)
 *	  / / / / ___/ __ `/ / __ `__ \/ /
 *	 / /_/ (__  ) /_/ / / / / / / / / 
 *	 \__, /____/\__,_/_/_/ /_/ /_/_/ 
 *	/____/	User: Youssef Salmi
 *			File: BufferRequest.hpp 
 */

#pragma once
#ifndef BUFFERREQUEST_HPP
#define BUFFERREQUEST_HPP

#include <iostream>
#include <string>

#include "src/Request/Request.hpp"

class BufferRequest : public Request
{
	std::string	mBuffer;

	BufferRequest( const BufferRequest& b);
	BufferRequest&	operator=( const BufferRequest& b );
public:
	BufferRequest(const Request& request, const std::string& buffer);
	~BufferRequest();

	int			getSocketFd() const;
	std::string	read();
	bool		done() const;

	void		dump() const;

};
#endif
