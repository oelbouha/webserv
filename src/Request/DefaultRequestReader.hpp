/*	                     __          _
 *	   __  ___________ _/ /___ ___  (_)
 *	  / / / / ___/ __ `/ / __ `__ \/ /
 *	 / /_/ (__  ) /_/ / / / / / / / / 
 *	 \__, /____/\__,_/_/_/ /_/ /_/_/ 
 *	/____/	User: Youssef Salmi
 *			File: DefaultRequestReader.hpp 
 */

#pragma once
#ifndef DEFAULTREQUESTREADER_HPP
#define DEFAULTREQUESTREADER_HPP

#include <iostream>

#include "IRequestReader.hpp"
#include "IClientSocket.hpp"

class DefaultRequestReader : public IRequestReader
{
	IClientSocket&	mSocket;
	size_t			mContentLength;
	size_t			mRead;

public:
	DefaultRequestReader(IClientSocket& sock, size_t contentLenght);

	std::string	read();
	bool		eof() const;
    size_t      getContentLength() const;

};
#endif
