/*	                     __          _
 *	   __  ___________ _/ /___ ___  (_)
 *	  / / / / ___/ __ `/ / __ `__ \/ /
 *	 / /_/ (__  ) /_/ / / / / / / / / 
 *	 \__, /____/\__,_/_/_/ /_/ /_/_/ 
 *	/____/	User: Youssef Salmi
 *			File: ChunkedWriter.hpp 
 */

#pragma once
#ifndef CHUNKEDWRITER_HPP
#define CHUNKEDWRITER_HPP

#include <iostream>

#include "IResponseWriter.hpp"
#include "src/Interfaces/IClientSocket.hpp"

#include "Utils.hpp"

class ChunkedWriter : public IResponseWriter
{
	const IClientSocket&	mSocket;
	std::string				mHeader;
	std::string				mActiveBuffer;
	std::string				mWaitingBuffer;
	bool					mEOF;
public:
	ChunkedWriter(const IClientSocket& socket);
	~ChunkedWriter();

	virtual void		setHeader(const std::string& header);
	virtual void		append(const std::string& data);
	virtual int			write();
	virtual bool		done() const;

};
#endif
