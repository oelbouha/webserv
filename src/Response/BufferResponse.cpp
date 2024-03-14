/*	                     __          _
 *	   __  ___________ _/ /___ ___  (_)
 *	  / / / / ___/ __ `/ / __ `__ \/ /
 *	 / /_/ (__  ) /_/ / / / / / / / /
 *	 \__, /____/\__,_/_/_/ /_/ /_/_/
 *	/____/	User: Youssef Salmi
 *			File: BufferResponse.cpp
 */

#include "BufferResponse.hpp"

BufferResponse::BufferResponse(const IClientSocket &aSocket) :
	AResponse(aSocket),
	mCursor(0)
{}

BufferResponse::BufferResponse(const BufferResponse&	aBufferResponse) :
	AResponse(aBufferResponse.mSocket),
	mCursor(0)
{}

BufferResponse::~BufferResponse() {}

BufferResponse&	BufferResponse::operator=(const BufferResponse&	aBufferResponse)
{
	if (this != &aBufferResponse)
	{

	}
	return (*this);
}

BufferResponse&	BufferResponse::setBody(const std::string &aBody)
{
	mBody = aBody;
	mHeaders["content-length"] = utils::to_string(mBody.length());
	return (*this);
}

// BufferResponse&	BufferResponse::setBodyFile(const std::string &aFileName)
// {
// 	(void)aFileName;
// 	return *this;
// }

BufferResponse&	BufferResponse::build()
{
	mRawResponse = "HTTP/1.1 " + AResponse::StatusCodes.at(mStatusCode) + "\r\n";

	for (string_string_map::iterator it = mHeaders.begin(); it != mHeaders.end(); ++it)
		mRawResponse += it->first + ": " + it->second + "\r\n";

	mRawResponse += "\r\n";

	mRawResponse += mBody;

	return (*this);
}

void BufferResponse::send()
{
	try
	{
		mCursor = mSocket.write(mRawResponse);
		mRawResponse.erase(0, mCursor);
	}
	catch (const SocketException &e)
	{
		std::cerr << e.what() << std::endl;
		mRawResponse.clear();
	}
}

bool BufferResponse::done() const
{
	return (mRawResponse.empty());
}