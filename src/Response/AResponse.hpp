/*	                     __          _
 *	   __  ___________ _/ /___ ___  (_)
 *	  / / / / ___/ __ `/ / __ `__ \/ /
 *	 / /_/ (__  ) /_/ / / / / / / / /
 *	 \__, /____/\__,_/_/_/ /_/ /_/_/
 *	/____/	User: Youssef Salmi
 *			File: AResponse.hpp
 */

#pragma once
#ifndef ARESPONSE_HPP
#define ARESPONSE_HPP

#include <iostream>
#include <fstream>
#include <cstddef>
#include <string>
#include <map>

#include <unistd.h>
#include <fcntl.h>

#include "IClientSocket.hpp"
#include "IResponse.hpp"

#include "src/Server/MimeTypes.hpp"
#include "src/Socket/SocketException.hpp"
#include "src/Response/ResponseException.hpp"

#include "Utils.hpp"
#include "Types.hpp"

class AResponse : public IResponse
{
protected:
	const IClientSocket&	mSocket;
	unsigned int 			mStatusCode;
	string_string_map		mHeaders;

	AResponse(const AResponse& aResponse);
	AResponse&	operator=(const AResponse& aResponse);

public:
	AResponse(const IClientSocket &aSocket);
	~AResponse();

	virtual int getSocketFd() const;

	AResponse&	setStatusCode(unsigned int aStatusCode);
	AResponse&	setHeader(const std::string &aHeader, const std::string &aValue);

	static const uint_string_map	StatusCodes;
	static uint_string_map 			initStatusCodes();
};

#endif
