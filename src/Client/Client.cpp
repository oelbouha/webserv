/*	                     __          _
 *	   __  ___________ _/ /___ ___  (_)
 *	  / / / / ___/ __ `/ / __ `__ \/ /
 *	 / /_/ (__  ) /_/ / / / / / / / /
 *	 \__, /____/\__,_/_/_/ /_/ /_/_/
 *	/____/	User: Youssef Salmi
 *			File: Client.cpp
 */

#include "Client.hpp"

int Client::KeepAliveMax = 60;
int Client::KeepAliveCount = 0;
int Client::KeepAliveTimeout = 40;

void Client::setKeepAlive(int maxConnections, int timeout)
{
    Client::KeepAliveMax = maxConnections;
    Client::KeepAliveTimeout = timeout;
}

Client::Client(IClientSocket *aSocket, int aIncomingIP, int aIncomingPort) :
    mIncomingIP(aIncomingIP),
    mIncomingPort(aIncomingPort),
    mSocket(aSocket),
    mRequest(NULL),
    mKeepAlive(false),
    mLastActivityEnd(std::time(NULL)),
    activeResponse(NULL),
    activeUpload(NULL),
    status(Client::CONNECTED)
{}

Client::~Client() {
    delete  mSocket;
    delete  mRequest;

    if (mKeepAlive)
        Client::KeepAliveCount--;
}

bool    Client::operator==(const IClient& client) const
{
    return (getSocketFd() == client.getSocketFd());
}

int     Client::getSocketFd() const
{
    return mSocket->getSocketFd();
}

bool    Client::hasRequest() const
{
    return ( mRequest != NULL );
}

bool    Client::hasTimedOut() const
{
    if (status == Client::ACTIVE) return false;
    if (status == Client::IDLE && !mKeepAlive) return true;
    
    long long inactiveTime = std::difftime(std::time(0), mLastActivityEnd);
    return (inactiveTime >= Client::KeepAliveTimeout);
}

void    Client::makeRequest()
{
    try
    {
        mRequest = new Request(*mSocket, mIncomingIP, mIncomingPort);
        mRequest->readHeader();
    }
    catch ( const RequestException& e ) { delete mRequest; mRequest = NULL; }
    catch ( const SocketException &e ) { delete mRequest; mRequest = NULL; throw e; }
}

void    Client::setKeepAlive( std::string connection )
{
    if (!mKeepAlive && Client::KeepAliveCount < Client::KeepAliveMax)
    {
        mKeepAlive = utils::str_to_lower(connection) == "keep-alive";
        Client::KeepAliveCount++;
    }
    else if (mKeepAlive && utils::str_to_lower(connection) == "close")
    {
        mKeepAlive = false;
        Client::KeepAliveCount--;
    }
    
}

Request* Client::getRequest()
{
    Request *ret = mRequest;
    mRequest = NULL;
    return (ret);
}

void    Client::resetTimeout()
{
    if (mKeepAlive)
        mLastActivityEnd = std::time(NULL);
}

void    Client::setResponseHeaders(IResponse* res) const
{
    if (mKeepAlive) {
        res->setHeader("Connection", "keep-alive");
        return;
    }
    res->setHeader("Connection", "close");
}

void    Client::setResponseHeaders(IProxyResponse* res) const
{
    if (mKeepAlive) {
        res->setHeader("Connection", "keep-alive");
        return;
    }
    res->setHeader("Connection", "close");
}