

#ifndef KQUEUEMULTIPLEXER_HPP
#define KQUEUEMULTIPLEXER_HPP

#include <map>
#include <queue>
#include <list>

#include <iostream>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/event.h>
#include <unistd.h>

#include "IMultiplexer.hpp"

#define MAX_EVENTS 10

class KqueueMultiplexer : public IMultiplexer {
	std::list<IServerSocket *> Servers;
	std::list<IClient *> Clients;
	std::list<IResponse *> Responses;
  public:
		KqueueMultiplexer();
		~KqueueMultiplexer();

		void add(IServerSocket &aServer);
		void remove(IServerSocket &aServer);

		void add(IClient &aClient);
		void remove(IClient &aClient);

		void add(IResponse &aResponse);
		void remove(IResponse &aResponse);

		void wait(unsigned long int timeout);
		void prepare();

		std::queue<IServerSocket *> getReadyServerSockets() const;
		std::queue<IClient *> getReadyClients() const;
		std::queue<IResponse *> getReadyResponses() const;
   
   private:
		struct kevent events[MAX_EVENTS];
		int		Kq;
		int 	Eventsnum;
		bool	fdsChanged;
};
#endif