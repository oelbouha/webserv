/*	                     __          _
 *	   __  ___________ _/ /___ ___  (_)
 *	  / / / / ___/ __ `/ / __ `__ \/ /
 *	 / /_/ (__  ) /_/ / / / / / / / / 
 *	 \__, /____/\__,_/_/_/ /_/ /_/_/ 
 *	/____/	User: Youssef Salmi
 *			File: DefaultWriter.cpp 
 */

#include "DefaultWriter.hpp"

DefaultWriter::DefaultWriter(const IClientSocket& socket, size_t content_length) :
	mSocket(socket),
	mContentLength(content_length)
{}

DefaultWriter::~DefaultWriter()
{}

void	DefaultWriter::setHeader(const std::string& header)
{
	mBuffer = header + mBuffer;
}

void	DefaultWriter::append(const std::string& data)
{
	mBuffer += data;
}

int	DefaultWriter::write()
{
	if (mBuffer.empty()) return 0;

	int r = mSocket.write(mBuffer);
	mBuffer.erase(0, r);
	mContentLength -= r;
	return r;
}

bool	DefaultWriter::done() const
{
	return mBuffer.empty();
}