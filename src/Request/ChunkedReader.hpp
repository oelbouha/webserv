/*	                     __          _
 *	   __  ___________ _/ /___ ___  (_)
 *	  / / / / ___/ __ `/ / __ `__ \/ /
 *	 / /_/ (__  ) /_/ / / / / / / / / 
 *	 \__, /____/\__,_/_/_/ /_/ /_/_/ 
 *	/____/	User: Youssef Salmi
 *			File: ChunkedReader.hpp 
 */

#pragma once
#ifndef ChunkedReader_HPP
#define ChunkedReader_HPP

#include <iostream>
#include <string>
#include <sstream>

#include "IRequestReader.hpp"
#include "IClientSocket.hpp"

#include "RequestException.hpp"

#include "Utils.hpp"

class ChunkedReader : public IRequestReader
{
	IClientSocket&	mSocket;
	std::string		mBuffer;
	size_t			mContentLength;
	size_t			mCurrentChunkSize;
	size_t			mCurrentChunkRead;
	bool			mEof;
	bool			mTrailer;

	// size_t			hexToInt(const std::string& raw) const;
	// std::string		unchunk(std::string& buffer);
	bool			parseChunkHeader(std::string& buffer);

public:
	ChunkedReader(IClientSocket& sock);

	std::string	read();
	bool		eof() const;
    size_t      getContentLength() const;

};
#endif
