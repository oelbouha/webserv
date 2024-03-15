/*	                     __          _
 *	   __  ___________ _/ /___ ___  (_)
 *	  / / / / ___/ __ `/ / __ `__ \/ /
 *	 / /_/ (__  ) /_/ / / / / / / / / 
 *	 \__, /____/\__,_/_/_/ /_/ /_/_/ 
 *	/____/	User: Youssef Salmi
 *			File: DefaultReader.cpp 
 */

#include "DefaultReader.hpp"

DefaultReader::DefaultReader(IClientSocket& sock, size_t contentLength):
    mSocket(sock),
    mContentLength(contentLength),
    mRead(0)
{}

size_t  DefaultReader::getContentLength() const
{
    return (mContentLength);
}

std::string	DefaultReader::read()
{
    if (eof())
        return ("");

    std::string ret = mSocket.read(mContentLength - mRead);
    
    mRead += ret.length();

    return (ret);
}

bool	DefaultReader::eof() const
{
    return (mRead == mContentLength);
}
