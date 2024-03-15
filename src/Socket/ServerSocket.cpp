/*	                     __          _
 *	   __  ___________ _/ /___ ___  (_)
 *	  / / / / ___/ __ `/ / __ `__ \/ /
 *	 / /_/ (__  ) /_/ / / / / / / / /
 *	 \__, /____/\__,_/_/_/ /_/ /_/_/
 *	/____/	User: Youssef Salmi
 *			File: ServerSocket.cpp
 */

#include "ServerSocket.hpp"
#include <iostream>

int ServerSocket::sBackLog = 5;

ServerSocket::ServerSocket() :
    mIP(utils::ip(0, 0, 0, 0)),
    mPort(8080)
{
    if ((mID = ::socket(AF_INET, SOCK_STREAM, 0)) < 0)
        throw SocketException("Can't create socket");
}

ServerSocket::ServerSocket(unsigned int aIP, unsigned short aPort) :
    mIP(aIP),
    mPort(aPort)
{
    if ((mID = ::socket(AF_INET, SOCK_STREAM, 0)) < 0)
        throw SocketException("Can't create socket");
}

ServerSocket::ServerSocket(const ServerSocket &s) : 
    mID(s.mID),
    mIP(s.mIP),
    mPort(s.mPort)
{}

ServerSocket::~ServerSocket() 
{}

ServerSocket &ServerSocket::operator=(const ServerSocket &aServerSocket)
{
    if (this != &aServerSocket) {
        mID = aServerSocket.mID;
        mIP = aServerSocket.mIP;
        mPort = aServerSocket.mPort;
    }
    return (*this);
}

void    ServerSocket::close()
{
    ::close(mID);
}

int ServerSocket::getSocketFd() const { return mID; }

void ServerSocket::bind()
{
    struct sockaddr_in  addr;
    std::size_t         addr_len = sizeof(addr);

    int opt = 1;
    if (::setsockopt(mID, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
        throw SocketException(std::string("Can't reuse addr/port ") +
                strerror(errno));
    if (::setsockopt(mID, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt)) < 0)
        throw SocketException(std::string("Can't reuse addr/port ") +
                strerror(errno));

    std::memset(&addr, 0, addr_len);
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = mIP;
    addr.sin_port = htons(mPort);

    if (::bind(mID, (struct sockaddr *)&addr, addr_len) < 0)
    {
        throw SocketException("Can't bind to " + utils::ip(ntohl(mIP)) + ":" +
                utils::to_string(mPort) + " " + strerror(errno));
    }
}

void ServerSocket::listen()
{
    if (::listen(mID, ServerSocket::sBackLog) < 0)
        throw SocketException("Can't listen on " + utils::ip(ntohl(mIP)) + ":" +
                utils::to_string(mPort) + " " + strerror(errno));
}

IClientSocket *ServerSocket::accept() const
{
    struct sockaddr_in addr;
    unsigned int  addr_len = sizeof(addr);

    int id = ::accept(mID, (struct sockaddr *)&addr, (socklen_t *)&addr_len);

    Logger::info ("Accepted: ")( utils::ip(ntohl(addr.sin_addr.s_addr)) )
        (":")( ntohs(addr.sin_port) )(" on ")( utils::ip(ntohl(mIP)) ).flush();
    Logger::debug ("Accepted: ")( utils::ip(ntohl(addr.sin_addr.s_addr)) )
        (":")( ntohs(addr.sin_port) )(" on ")( utils::ip(ntohl(mIP)) ).flush();

    return (new ClientSocket(id));
}

void ServerSocket::setNonBlocking()
{
    if (fcntl(mID, F_SETFL, O_NONBLOCK, O_CLOEXEC) < 0)
        throw SocketException("Can't switch socket to non-blocking mode");
}

int ServerSocket::getPort() const { return (mPort); }

ip_t ServerSocket::getIP() const { return (mIP); }

