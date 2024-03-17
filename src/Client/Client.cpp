/*	                     __          _
 *	   __  ___________ _/ /___ ___  (_)
 *	  / / / / ___/ __ `/ / __ `__ \/ /
 *	 / /_/ (__  ) /_/ / / / / / / / /
 *	 \__, /____/\__,_/_/_/ /_/ /_/_/
 *	/____/	User: Youssef Salmi
 *			File: Client.cpp
 */

#include "Client.hpp"
#include <ctime>

int Client::KeepAliveMax = 2;
int Client::KeepAliveCount = 0;
int Client::KeepAliveTimeout = 40;

void Client::setKeepAlive(int maxConnections, int timeout) {
    Client::KeepAliveMax = maxConnections;
    Client::KeepAliveTimeout = timeout;
}


Client::Client(IClientSocket *aSocket, int aIncomingIP, int aIncomingPort) :
    mIncomingIP(aIncomingIP),
    mIncomingPort(aIncomingPort),
    mSocket(aSocket),
    mRequest(NULL),
    mKeepAlive(false),
    activeResponse(NULL),
    activeUpload(NULL),
    status(Client::CONNECTED),
    lastActivityEnd(std::time(NULL))
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

const IClientSocket&    Client::getSocket() const
{
    return (*mSocket);
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
    if (status != Client::IDLE && status != Client::CONNECTED)
        return false;
    
    long long inactiveTime = std::difftime(std::time(0), lastActivityEnd);
    return (inactiveTime >= Client::KeepAliveTimeout);
}

void Client::makeRequest()
{
    Request*   req;
    try
    {
        req = new Request(*mSocket, mIncomingIP, mIncomingPort);
        req->readHeader();
        mRequest = req;

        if (!mKeepAlive && Client::KeepAliveCount < Client::KeepAliveMax)
        {
            std::string connection = req->getHeader("connection");
            mKeepAlive = utils::str_to_lower(connection) == "keep-alive";
            Client::KeepAliveCount++;
        }
    }
    catch ( const RequestException& e ) { delete req; }
    catch ( const SocketException &e ) { delete req; throw e; }
}

Request *Client::getRequest()
{
    Request *ret = mRequest;
    mRequest = NULL;
    return (ret);
}

bool            Client::isKeptAlive() const
{
    return status != Client::IDLE || mKeepAlive;
}

void            Client::resetTimeout()
{
    if (mKeepAlive)
        lastActivityEnd = std::time(NULL);
}

void              Client::setResponseHeaders(IResponse* res) const
{
    if (mKeepAlive) {
        res->setHeader("connection", "keep-alive");
        return;
    }
    res->setHeader("connection", "close");
}

// void              Client::setClientHeaders(IProxyResponse* res) const
// {
//     if (mKeepAlive) {
//         res->setHeader("connection", "keep-alive");
//         return;
//     }
//     res->setHeader("connection", "close");
// }