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
    mRequest(NULL)
{}

Client::~Client()
{
    delete  mSocket;
    delete  mRequest;
}

bool    Client::operator==(const IClient& client) const
{
    return (this->getSocketFd() == client.getSocketFd());
}

int     Client::getSocketFd() const { return mSocket->getSocketFd(); }

int     Client::getIncomingIP() const { return mIncomingIP; }

int     Client::getIncomingPort() const { return mIncomingPort; }

bool    Client::hasRequest() const { return (mRequest != NULL); }

bool    Client::hasClosedTheConnection() const { return (mHasClosedTheConnection); }

void Client::makeRequest() 
{ 
    try
    {
        if (mRequest == NULL)
            mRequest = new Request(*mSocket, mIncomingIP, mIncomingPort);
        mRequest->build();

    } catch(const RequestException& e)
    {
        if (e.error == RequestException::CONNECTION_COLOSED)
        {
            mHasClosedTheConnection = true;
            std::cout << "client closed connection\n" << std::flush;
        }
    }
    catch (const SocketException &e) {
    }
}

IRequest *Client::getRequest()
{
  IRequest *ret = mRequest;
  mRequest = NULL;
  return (ret);
}

void    Client::dump()
{
    // mSocket->dump();
    if (!mRequest){
        makeRequest();

        if (mHasClosedTheConnection)
        {
            delete  mRequest;
            mRequest = NULL;
            return;
        }

        mRequest->dump();
    }
    std::cout << "dumping...\n" << std::flush;
    std::cout << mRequest->read() << std::flush;
    if (mRequest->done()){
        std::cout << "request complete - content length: " 
            << mRequest->getContentLength() << "\n" << std::flush;
        delete mRequest;
        mRequest = NULL;
        throw 42;
    }
}
