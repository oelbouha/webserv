/*	                     __          _
 *	   __  ___________ _/ /___ ___  (_)
 *	  / / / / ___/ __ `/ / __ `__ \/ /
 *	 / /_/ (__  ) /_/ / / / / / / / /
 *	 \__, /____/\__,_/_/_/ /_/ /_/_/
 *	/____/	User: Youssef Salmi
 *			File: ClientSocket.cpp
 */

#include "ClientSocket.hpp"
#include <string>

ClientSocket::ClientSocket(int aFileDes):
    mID(aFileDes),
    mRead(-1)
{
    // std::cout << "client socket +++++++++++++++++++++++++++++++++ opened " << mID << std::endl;
}

ClientSocket::ClientSocket(const ClientSocket &aClientSocket):
    mID(aClientSocket.mID)
{}

ClientSocket::~ClientSocket()
{
    // std::cout << "client socket +++++++++++++++++++++++++++++++++ closing " << mID << std::endl;
    ::close(mID);
}

ClientSocket &ClientSocket::operator=(const ClientSocket &aClientSocket)
{
    if (this != &aClientSocket) {
    }
    return (*this);
}

int ClientSocket::getSocketFd() const { return mID; }


int ClientSocket::write( const std::string& aBuffer ) const
{
    int r = ::write(mID, aBuffer.data(), aBuffer.length());

    if (r < 0){
        perror("socket write");
        throw SocketException(std::string("Couldn't write to socket: ") + strerror(errno));
    }

    return (r);
}

int ClientSocket::writeAll( const std::string& aBuffer ) const
{
    int         r = 0;
    const char* buffer = aBuffer.data();
    size_t      size = aBuffer.length();

    while (true)
    {
        int w = ::write(mID, buffer + r, size - r);
        if (w <= 0)
            break ;
        r += w;
    }
    return (r);
}

std::string ClientSocket::read(unsigned int aMaxSize) 
{
    mBuffer = readAll();

    std::string ret = mBuffer.substr(0, aMaxSize);

    if (mBuffer.length() > aMaxSize)
        mBuffer.erase(0, aMaxSize);
    else
        mBuffer.clear();
    return (ret);
}

std::string ClientSocket::readHeaderOnly() 
{
    mBuffer = readAll();

    if (mBuffer.empty())
        return (mBuffer);

    std::size_t pos = mBuffer.find("\r\n\r\n");

    if (pos == std::string::npos)
        return ("");

    std::string ret = mBuffer.substr(0, pos);
    mBuffer.erase(0, pos + 4);

    return (ret);
}

std::string ClientSocket::readAll()
{
    std::string   ret = mBuffer;
    std::size_t   bufferSize = 4096;
    char          buffer[bufferSize];
    int           r;

    mBuffer.clear();

    while ((r = ::read(mID, buffer, bufferSize)) > 0)
        ret += std::string(buffer, r);

    if (r != -1)
        mRead = r;

    if (mRead == 0 && ret.empty())
        throw SocketException("Connection Closed");

    return (ret);
}

void ClientSocket::dump() 
{
    std::string s = readAll();

    for (std::string::iterator it = s.begin(); it != s.end(); ++it)
    {
        if (*it != 10 && (*it < 32 || *it > 126))
            std::cout << "▒";
        else
            std::cout << *it;
    }
    std::cout << std::endl << std::flush;
}

void ClientSocket::setNonBlocking()
{
    if (fcntl(mID, F_SETFL, O_NONBLOCK, O_CLOEXEC) < 0)
        throw SocketException("Can't switch socket to non-blocking mode");
}

