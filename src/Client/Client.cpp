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
      mHasRequest(false)
{}

Client::~Client() {}

int Client::getID() const { return mSocket->getID(); }

int Client::getIncomingIP() const { return mIncomingIP; }

int Client::getIncomingPort() const { return mIncomingPort; }

void Client::makeRequest() {
  try {
    mRequest = new Request(*mSocket, mIncomingIP, mIncomingPort);
    mRequest->build();
    mHasRequest = true;
  } catch (const SocketException &e) {
  }
}

bool Client::hasRequest() const { return mHasRequest; }

IRequest *Client::getRequest() {
   return (mRequest); 
}
