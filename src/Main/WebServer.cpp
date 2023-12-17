/*	                     __          _
 *	   __  ___________ _/ /___ ___  (_)
 *	  / / / / ___/ __ `/ / __ `__ \/ /
 *	 / /_/ (__  ) /_/ / / / / / / / /
 *	 \__, /____/\__,_/_/_/ /_/ /_/_/
 *	/____/	User: Youssef Salmi
 *			File: WebServer.cpp
 */

#include "WebServer.hpp"
#include <queue>

WebServer::WebServer() : mConfig(NULL) {}

WebServer::WebServer(const Config *aConfig) : mConfig(aConfig) {}

WebServer::~WebServer() {}

void WebServer::start() {
  mSocket.bind();
  mSocket.listen();
}

void WebServer::loop() 
{
    std::queue<IServerSocket*>  qs;
    std::queue<IClient*>        qc;
    std::queue<IResponse*>      qr;

    while (true){
        mMux->wait(1000);

        qs = mMux->getReadyServerSockets();
        qc = mMux->getReadyClients();
        qr = mMux->getReadyResponses();

        while (qs.size())
        {
            const IServerSocket*    sock = qs.front();
            IClientSocket*          clientSock = sock->accept();
            Client                  client(clientSock, sock->getIP(), sock->getPort());

            mClients.push_back(client);
            mMux->add(client);
            qs.pop();
        }
        while (qc.size())
        {
            IClient*  client = qc.front();

            // if (client in cgi)
            //  set request active
            //  else
            client->makeRequest();
            if (client->hasRequest())
                mRequests.push(client->getRequest());

            qc.pop();
        }
        /*
         *
         * while (cgi pin)
         *  set pin active
         *  if (cgi ready)
         *   send to cgi
         *
         * while (cgi pout)
         *  set pout cgi active
         */

        while (qr.size())
        {
            IResponse*  res = qr.front();

            res->send();
            if (res->isSendingComplete())
                // remove from mResponses;
            qr.pop();
        }
        while (mRequests.size())
        {
            IRequest*   req = mRequests.front();

            IResponse*  res = mServers.handle(req);

            mResponses.push_back(res);
        }
    }
}

/*
while (true)
{
        // code

        // sending responses section
        for res in responses that are ready
        {
                if (res.isSendingComplete())
                        remove res from responses
                        check if connection header is "close"
                                close client socket
                        else
                                update time out
                res.send();
        }

        // keep alive timeout seciton
        loop:
                close timed out clients
}

*/
