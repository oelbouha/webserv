
#include "KqueueMultiplexer.hpp"

KqueueMultiplexer::~KqueueMultiplexer()
{
	close(Kq);
}

KqueueMultiplexer::KqueueMultiplexer()
{
	fdsChanged = true;
    Kq = kqueue();
	if (Kq < 0)
	{
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
	struct kevent changes;

	std::list<IServerSocket*>::iterator it = std::find(Servers.begin(),
		Servers.end(), &server);
	if (it == Servers.end())
		return ;
	IServerSocket *Server = *it;
	EV_SET(&changes, Server->getID(), EVFILT_READ, EV_DELETE, 0, 0, NULL);
	if (kevent(Kq, &changes, 1, NULL, 0, NULL) == -1)
		perror("kevent (EV_DELETE)");	
	Servers.erase(it);
    fdsChanged = true;
}

std::queue<IServerSocket *> KqueueMultiplexer::getReadyServerSockets() const{
	std::queue<IServerSocket *> result;
	
    if (ReadyEvents == 0)
        return result;
    std::list<IServerSocket *>::const_iterator it = Servers.begin();
	while (it != Servers.end())
	{
        IServerSocket *server = *it;
        for (int i = 0; i < ReadyEvents; ++i)
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

	std::list<IClient*>::iterator it = std::find(Clients.begin(),
		Clients.end(), &client);
	if (it == Clients.end())
		return ;
	IClient *Client = *it;
	EV_SET(&changes, Client->getID(), EVFILT_READ, EV_DELETE, 0, 0, NULL);
	if (kevent(Kq, &changes, 1, NULL, 0, NULL) == -1)
		perror("kevent (EV_DELETE)");
	Clients.erase(it);
    fdsChanged = true;
}

std::queue<IClient *> KqueueMultiplexer::getReadyClients() const{
	std::queue<IClient *> result;

	if (ReadyEvents == 0)
        return result;
	std::list<IClient*>::const_iterator it = Clients.begin();
	while (it != Clients.end())
	{
        IClient *client = *it;
        for (int i = 0; i < ReadyEvents; ++i)
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

	std::list<IResponse*>::iterator it = std::find(Responses.begin(),
		Responses.end(), &response);
	if (it == Responses.end())
		return ;
	IResponse* res = *it;
	EV_SET(&changes, res->getID(), EVFILT_WRITE, EV_DELETE, 0, 0, NULL);
	if (kevent(Kq, &changes, 1, NULL, 0, NULL) == -1)
		perror("kevent EV_DELETE");
	Responses.erase(it);
    fdsChanged = true;
}

std::queue<IResponse *> KqueueMultiplexer::getReadyResponses() const{
	std::queue<IResponse *> result;

    if (ReadyEvents == 0)
        return result;

	std::list<IResponse* >::const_iterator it = Responses.begin();
	while (it != Responses.end())
	{
        IResponse *res = *it;
        for (int i = 0; i < ReadyEvents; ++i)
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
	
	ReadyEvents = kevent(Kq, NULL, 0, events, MAX_EVENTS, &timeout);
}

void	KqueueMultiplexer::prepareClients(){
	struct kevent changes;
	std::list<IClient* >::iterator itc = Clients.begin();
	while (itc != Clients.end())
	{
		IClient* client = *itc;
		EV_SET(&changes, client->getID(), EVFILT_READ, EV_ADD, 0, 0, NULL);
		if (kevent(Kq, &changes, 1, NULL, 0, NULL) == -1)
        {
            perror("kevent");
            close(Kq);
            // exit(EXIT_FAILURE);
        }
		++itc;
	}
}

void	KqueueMultiplexer::prepareResponses(){
	struct kevent changes;
	std::list<IResponse* >::iterator its = Responses.begin();
	while (its != Responses.end()){
		IResponse* res = *its;
		EV_SET(&changes, res->getID(), EVFILT_WRITE, EV_ADD, 0, 0, NULL);
		if (kevent(Kq, &changes, 1, NULL, 0, NULL) == -1)
        {
            perror("kevent");
            close(Kq);
            // exit(EXIT_FAILURE);
        }
		++its;
	}
}

void	KqueueMultiplexer::prepareServers(){
	struct kevent changes;
	std::list<IServerSocket* >::iterator it = Servers.begin();
	while (it != Servers.end())
	{
		IServerSocket* server = *it;
		EV_SET(&changes, server->getID(), EVFILT_READ, EV_ADD, 0, 0, NULL);
		if (kevent(Kq, &changes, 1, NULL, 0, NULL) == -1)
        {
            perror("kevent");
            close(Kq);
            // exit(EXIT_FAILURE);
        }
		++it;
	}
}
void	KqueueMultiplexer::prepare(){
    if (fdsChanged == false)
        return ;

	prepareServers();
	prepareClients();
	prepareResponses();

    fdsChanged = false;
}

