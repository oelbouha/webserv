/*	                     __          _
 *	   __  ___________ _/ /___ ___  (_)
 *	  / / / / ___/ __ `/ / __ `__ \/ /
 *	 / /_/ (__  ) /_/ / / / / / / / / 
 *	 \__, /____/\__,_/_/_/ /_/ /_/_/ 
 *	/____/	User: Youssef Salmi
 *			File: DefaultReader.hpp 
 */

#pragma once
#ifndef DefaultReader_HPP
#define DefaultReader_HPP

#include <iostream>

#include "IRequestReader.hpp"
#include "IClientSocket.hpp"

class DefaultReader : public IRequestReader
{
	IClientSocket&	mSocket;
	size_t			mContentLength;
	size_t			mRead;

public:
	DefaultReader(IClientSocket& sock, size_t contentLenght);

	std::string	read();
	bool		eof() const;
    size_t      getContentLength() const;

};
#endif
