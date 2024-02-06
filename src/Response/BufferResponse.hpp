/*	                     __          _
 *	   __  ___________ _/ /___ ___  (_)
 *	  / / / / ___/ __ `/ / __ `__ \/ /
 *	 / /_/ (__  ) /_/ / / / / / / / /
 *	 \__, /____/\__,_/_/_/ /_/ /_/_/
 *	/____/	User: Youssef Salmi
 *			File: BufferResponse.hpp
 */

#pragma once
#ifndef BUFFERRESPONSE_HPP
#define BUFFERRESPONSE_HPP

#include <iostream>
#include <map>
#include <fstream>

#include <unistd.h>
#include <fcntl.h>

#include "IClientSocket.hpp"
#include "IResponse.hpp"

#include "src/Response/Response.hpp"
#include "src/Server/MimeTypes.hpp"
#include "src/Socket/SocketException.hpp"
#include "src/Response/ResponseException.hpp"

#include "Utils.hpp"
#include "Types.hpp"

class BufferResponse : public IResponse
{
	const IClientSocket&	mSocket;
	unsigned int 			mStatusCode;
	string_string_map		mHeaders;

	std::string mBody;
	std::string mRawResponse;

	size_t mCursor;

	BufferResponse(const BufferResponse &aBufferResponse);
	BufferResponse &operator=(const BufferResponse &aBufferResponse);

public:
	BufferResponse(const IClientSocket &aSocket);
	~BufferResponse();

	virtual int 	getSocketFd() const;

	BufferResponse&	setStatusCode(unsigned int aStatusCode);
	BufferResponse&	setHeader(const std::string &aHeader, const std::string &aValue);
	BufferResponse&	setBody(const std::string &aBody);
	BufferResponse&	setBodyFile(const std::string &aFileName);

	BufferResponse &build();

	void send();
	bool done() const;

	void dump();
};
#endif
