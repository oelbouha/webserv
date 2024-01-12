
#include "KqueueMultiplexer.hpp"

KqueueMultiplexer::~KqueueMultiplexer(){}

KqueueMultiplexer::KqueueMultiplexer(){
	fdsChanged = true;
    Kq = kqueue();
	if (Kq < 0){
		perror("Kqueue");
		// exit (EXIT_FAILURE);
	}
}

/*------------------------     Handling server     ---------------------*/

void	KqueueMultiplexer::add(IServerSocket& server){
	Servers.push_back(&server);
	fdsChanged = true;
}

void	KqueueMultiplexer::remove(IServerSocket& server){
	std::list<IServerSocket*>::iterator it = std::find(Servers.begin()
		, Servers.end(), &server);
	if (it != Servers.end())
		Servers.erase(it);
    fdsChanged = true;
}

std::queue<IServerSocket *> KqueueMultiplexer::getReadyServerSockets() const{
	std::queue<IServerSocket *> result;
	
    if (Eventsnum == 0)
        return result;
    
    std::cout << "servers size --> " << Servers.size() << std::endl;
    std::list<IServerSocket *>::const_iterator it = Servers.begin();
	while (it != Servers.end()){
        IServerSocket *server = *it;
        for (int i = 0; i < Eventsnum; ++i)
        {
            short filter = events[i].filter;
            int fd = events[i].ident;
            if (server->getID() == fd && filter == EVFILT_READ)
                result.push(server);
        }
		++it;
	} 
	return result;
}

/*------------------------     Handling client     ---------------------*/

void	KqueueMultiplexer::add(IClient& client){
	Clients.push_back(&client);
	fdsChanged = true;
}

void	KqueueMultiplexer::remove(IClient& client){
	struct kevent changes;
	IClient *Client;

	std::list<IClient*>::iterator it = std::find(Clients.begin(),
		Clients.end(), &client);
	if (it == Clients.end())
		return ;
	Client = *it;
	EV_SET(&changes, Client->getID(), EVFILT_READ, EV_DELETE, 0, 0, NULL);
	if (kevent(Kq, &changes, 1, NULL, 0, NULL) == -1)
		perror("kevent (EV_DELETE)");
	Clients.erase(it);
    fdsChanged = true;
}

std::queue<IClient *> KqueueMultiplexer::getReadyClients() const{
	std::queue<IClient *> result;

	if (Eventsnum == 0)
        return result;
	std::list<IClient*>::const_iterator it = Clients.begin();
	while (it != Clients.end()){
        IClient *client = *it;
        for (int i = 0; i < Eventsnum; ++i)
        {
            short filter = events[i].filter;
            int fd = events[i].ident;
            if (client->getID() == fd && filter == EVFILT_READ)
                result.push(client);
        }
		++it;
	} 
	return result;
}

/*------------------------     Handling response     ---------------------*/

void	KqueueMultiplexer::add(IResponse& response){
	Responses.push_back(&response);
    fdsChanged = true;
}

void	KqueueMultiplexer::remove(IResponse& response){
	struct kevent changes;
	IResponse* res;

	std::list<IResponse*>::iterator it = std::find(Responses.begin(),
		Responses.end(), &response);
	if (it == Responses.end())
		return ;
	res = *it;
	EV_SET(&changes, res->getID(), EVFILT_WRITE, EV_DELETE, 0, 0, NULL);
	if (kevent(Kq, &changes, 1, NULL, 0, NULL) == -1)
		perror("kevent EV_DELETE");
	Responses.erase(it);
    fdsChanged = true;
}

std::queue<IResponse *> KqueueMultiplexer::getReadyResponses() const{
	std::queue<IResponse *> result;

    if (Eventsnum == 0)
        return result;

	std::list<IResponse* >::const_iterator it = Responses.begin();
	while (it != Responses.end()){
        IResponse *res = *it;
        for (int i = 0; i < Eventsnum; ++i)
        {
            short filter = events[i].filter;
            int fd = events[i].ident;
            if (res->getID() == fd && filter == EVFILT_WRITE)
                result.push(res);
        }
		++it;
	} 
	return result;
}

/*---------------------------------------------------------------------*/

void	KqueueMultiplexer::wait(unsigned long int time){
    
    prepare();

	struct timespec timeout;
  	timeout.tv_sec = static_cast<long>(time / 1000000);
  	timeout.tv_nsec = static_cast<long>(time % 1000000);
	
	Eventsnum = kevent(Kq, NULL, 0, events, MAX_EVENTS, &timeout);
    std::cout << "event number -> " << Eventsnum << std::endl;
		// if (Eventsnum == -1) {
		// 	perror("kevent");
		// 	break;
		// }
}

void	KqueueMultiplexer::prepare(){
    struct kevent changes;

    if (fdsChanged == false)
        return ;

	std::list<IServerSocket* >::iterator it = Servers.begin();
	while (it != Servers.end()){
		IServerSocket* server = *it;
		EV_SET(&changes, server->getID(), EVFILT_READ, EV_ADD, 0, 0, NULL);
		if (kevent(Kq, &changes, 1, NULL, 0, NULL) == -1)
        {
            perror("kevent");
            close(Kq);
            exit(EXIT_FAILURE);
        }
		++it;
	}

	std::list<IClient* >::iterator itc = Clients.begin();
	while (itc != Clients.end()){
		IClient* client = *itc;
		EV_SET(&changes, client->getID(), EVFILT_READ, EV_ADD, 0, 0, NULL);
		if (kevent(Kq, &changes, 1, NULL, 0, NULL) == -1)
        {
            perror("kevent");
            close(Kq);
            exit(EXIT_FAILURE);
        }
		++itc;
	}

	std::list<IResponse* >::iterator its = Responses.begin();
	while (its != Responses.end()){
		IResponse* res = *its;
		EV_SET(&changes, res->getID(), EVFILT_WRITE, EV_ADD, 0, 0, NULL);
		if (kevent(Kq, &changes, 1, NULL, 0, NULL) == -1)
        {
            perror("kevent");
            close(Kq);
            exit(EXIT_FAILURE);
        }
		++its;
	}
    fdsChanged = false;
}

