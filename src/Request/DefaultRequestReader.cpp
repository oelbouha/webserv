/*	                     __          _
 *	   __  ___________ _/ /___ ___  (_)
 *	  / / / / ___/ __ `/ / __ `__ \/ /
 *	 / /_/ (__  ) /_/ / / / / / / / / 
 *	 \__, /____/\__,_/_/_/ /_/ /_/_/ 
 *	/____/	User: Youssef Salmi
 *			File: DefaultRequestReader.cpp 
 */

#include "DefaultRequestReader.hpp"

DefaultRequestReader::DefaultRequestReader(IClientSocket& sock, size_t contentLength):
    mSocket(sock),
    mContentLength(contentLength),
    mRead(0)
{}

size_t  DefaultRequestReader::getContentLength() const
{
    return (mContentLength);
}

std::string	DefaultRequestReader::read()
{
    if (eof())
        return ("");

    std::string ret = mSocket.read(mContentLength - mRead);
    
    mRead += ret.length();

    return (ret);
}

bool	DefaultRequestReader::eof() const
{
    return (mRead == mContentLength);
}
