/*	                     __          _
 *	   __  ___________ _/ /___ ___  (_)
 *	  / / / / ___/ __ `/ / __ `__ \/ /
 *	 / /_/ (__  ) /_/ / / / / / / / /
 *	 \__, /____/\__,_/_/_/ /_/ /_/_/
 *	/____/	User: Youssef Salmi
 *			File: Client.cpp
 */

#include "Client.hpp"

const IClientSocket&    Client::getSocket() const { return (*mSocket); }

Client::Client(IClientSocket *aSocket, int aIncomingIP, int aIncomingPort) :
    mIncomingIP(aIncomingIP),
    mIncomingPort(aIncomingPort),
    mSocket(aSocket),
    activeResponse(NULL),
    activeUpload(NULL),
    status(Client::CONNECTED)
{}

Client::~Client()
{
    delete  mSocket;
    while (mRequests.size())
    {
        delete  mRequests.front();
        mRequests.pop();
    }
}

bool    Client::operator==(const IClient& client) const
{
    return (this->getSocketFd() == client.getSocketFd());
}


int     Client::getSocketFd() const { return mSocket->getSocketFd(); }

bool    Client::hasRequest() const { return ( ! mRequests.empty() ); }

bool    Client::hasTimedOut(size_t timeout) const
{
    return (std::difftime(std::time(0), lastActivityEnd) > timeout);
}

void Client::makeRequest() 
{
    IRequest*   req;
    try
    {
        std::cout << "making request\n" << std::flush;
        
        req = new Request(*mSocket, mIncomingIP, mIncomingPort);
        
        std::cout << "building\n" << std::flush;

        req->build();
        mRequests.push(req);

    } catch(const RequestException& e)
    {
        if (e.error == RequestException::CONNECTION_CLOSED)
        {
            status = Client::DISCONNECTED;
            std::cout << "client closed connection\n" << std::flush;
        }
        else {
            std::cout << e.what() << std::endl;
        }
        delete req;
    }
    catch (const SocketException &e) {
    }
}

IRequest *Client::getRequest()
{
  IRequest *ret = mRequests.front();
  mRequests.pop();
  return (ret);
}
