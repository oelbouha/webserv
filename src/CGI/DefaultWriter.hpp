/*	                     __          _
 *	   __  ___________ _/ /___ ___  (_)
 *	  / / / / ___/ __ `/ / __ `__ \/ /
 *	 / /_/ (__  ) /_/ / / / / / / / / 
 *	 \__, /____/\__,_/_/_/ /_/ /_/_/ 
 *	/____/	User: Youssef Salmi
 *			File: DefaultWriter.hpp 
 */

#pragma once
#ifndef DEFAULTWRITER_HPP
#define DEFAULTWRITER_HPP

#include <iostream>

#include "IResponseWriter.hpp"
#include "src/Interfaces/IClientSocket.hpp"

class DefaultWriter : public IResponseWriter
{
	const IClientSocket&	mSocket;
	std::string				mBuffer;
	std::size_t				mContentLength;
public:
	DefaultWriter(const IClientSocket& socket, size_t cl);
	~DefaultWriter();

	virtual void		setHeader(const std::string& header);
	virtual void		append(const std::string& data);
	virtual int			write();
	virtual bool		done() const;

};
#endif
