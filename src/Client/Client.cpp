/*	                     __          _
 *	   __  ___________ _/ /___ ___  (_)
 *	  / / / / ___/ __ `/ / __ `__ \/ /
 *	 / /_/ (__  ) /_/ / / / / / / / /
 *	 \__, /____/\__,_/_/_/ /_/ /_/_/
 *	/____/	User: Youssef Salmi
 *			File: Client.cpp
 */

#include "Client.hpp"

Client::Client(IClientSocket *aSocket, int aIncomingIP, int aIncomingPort)
    : mIncomingIP(aIncomingIP), mIncomingPort(aIncomingPort), mSocket(aSocket),
      mHasRequest(false), mRequest(NULL) {}

Client::~Client() {}

bool    Client::operator==(const IClient& client) const
{ return (this->getID() == client.getID()); }

int Client::getID() const { return mSocket->getID(); }

int Client::getIncomingIP() const { return mIncomingIP; }

int Client::getIncomingPort() const { return mIncomingPort; }

void Client::makeRequest() {
  try {
    if (mRequest == NULL)
      mRequest = new Request(*mSocket, mIncomingIP, mIncomingPort);

    mRequest->build();
    mHasRequest = true;

  } catch(const RequestException& e)
  {
      if (e.error == RequestException::CONNECTION_COLOSED)
        mHasClosedTheConnection = true;
  }
  catch (const SocketException &e) {
  }
}

bool    Client::hasClosedTheConnection() const
{
    return (mHasClosedTheConnection);
}

bool Client::hasRequest() const { return mHasRequest; }

IRequest *Client::getRequest() {
  IRequest *ret = mRequest;
  mRequest = NULL;
  return (ret);
}
