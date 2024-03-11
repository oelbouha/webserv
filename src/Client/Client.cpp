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
int Client::KeepAliveTimeout = 60;

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
    if (status != Client::CONNECTED)
        return false;
        
    long long inactiveTime = std::difftime(std::time(0), lastActivityEnd);
    return (inactiveTime >= Client::KeepAliveTimeout);
}

void Client::makeRequest()
{
    IRequest*   req;
    try
    {
        req = new Request(*mSocket, mIncomingIP, mIncomingPort);
        req->build();
        mRequest = req;

    } catch(const RequestException& e)
    {
        if (e.error == RequestException::CONNECTION_CLOSED)
        {
            status = Client::DISCONNECTED;
        }
        else {
            std::cout << e.what() << std::endl;
        }
        delete req;
    } catch (const SocketException &e)
    {
        delete req;
    }
}

IRequest *Client::getRequest()
{
  IRequest *ret = mRequest;
  mRequest = NULL;
  return (ret);
}
