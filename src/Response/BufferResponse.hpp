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

#include "src/Response/AResponse.hpp"

class BufferResponse : public AResponse
{
	std::string mBody;
	std::string mRawResponse;
	size_t 		mCursor;

	BufferResponse(const BufferResponse &aBufferResponse);
	BufferResponse &operator=(const BufferResponse &aBufferResponse);

public:
	BufferResponse(const IClientSocket &aSocket);
	~BufferResponse();

	BufferResponse&	setBody(const std::string &aBody);
	BufferResponse&	setBodyFile(const std::string &aFileName);
	BufferResponse&	build();

	void send();
	bool done() const;
};
#endif
