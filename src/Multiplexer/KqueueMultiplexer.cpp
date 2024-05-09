/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   KqueueMultiplexer.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelbouha <oelbouha@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 15:10:02 by oelbouha          #+#    #+#             */
/*   Updated: 2024/05/09 16:28:37 by oelbouha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "KqueueMultiplexer.hpp"

KqueueMultiplexer::~KqueueMultiplexer(){
	close (Kq);
}

KqueueMultiplexer::KqueueMultiplexer() : set(4094)
{
    Kq = kqueue();
	if (Kq < 0)
	{
		perror("Kqueue");
		// exit (EXIT_FAILURE);
	}
}

/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
			Server																	 
::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/

void	KqueueMultiplexer::add(IServerSocket* server)
{
	Servers.push_back(server);
	AddOrDeleteEvent(server->getSocketFd(), EVFILT_READ, EV_ADD);
}

void	KqueueMultiplexer::remove(IServerSocket* server)
{
	std::list<IServerSocket*>::iterator pos = std::find(Servers.begin(), Servers.end(), server);

	if (pos != Servers.end())
	{
		AddOrDeleteEvent(server->getSocketFd(), EVFILT_READ, EV_DELETE);
		Servers.erase(pos);
	}
}

std::queue<IServerSocket *> KqueueMultiplexer::getReadyServerSockets() const 
{
	std::queue<IServerSocket *> ret;
	
    if (ReadyEvents == 0)
        return ret;

    std::list<IServerSocket *>::const_iterator it = Servers.begin();
	while (it != Servers.end())
	{
        IServerSocket *server = *it;
		if (IsEventSet(server->getSocketFd(), EVFILT_READ))
            ret.push(server);
		++it;
	} 
	return ret;
}

/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
			Client
::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/

void	KqueueMultiplexer::add(IClient* client)
{
	for (std::list<IClient*>::iterator it = Clients.begin(); it != Clients.end(); ++it)
		if ((*it)->getSocketFd() == client->getSocketFd())
			return;
	Clients.push_back(client);
	AddOrDeleteEvent(client->getSocketFd(), EVFILT_READ, EV_ADD);
	
	Logger::info("adding client ")(client->getSocketFd()).flush();
}

void	KqueueMultiplexer::remove(IClient* client)
{
	std::list<IClient*>::iterator pos = std::find(Clients.begin(), Clients.end(), client);
	Logger::info("client passed to remove ")(client->getSocketFd()).flush();

	if (pos == Clients.end())
		Logger::info("client not found: ")(client->getSocketFd()).flush();

	if (pos != Clients.end())
	{
		Logger::info("client is being removed ")((*pos)->getSocketFd()).flush();
		Clients.erase(pos);
		AddOrDeleteEvent(client->getSocketFd(), EVFILT_READ, EV_DELETE);
	}
}

std::queue<IClient *> KqueueMultiplexer::getReadyClients() const
{
	Logger::info("Mux client size: ")(Clients.size()).flush();
	std::queue<IClient *> result;

	if (ReadyEvents == 0)
        return result;
	
	std::list<IClient*>::const_iterator it = Clients.begin();
	while (it != Clients.end())
	{
        IClient *client = *it;
		if (IsEventSet(client->getSocketFd(), EVFILT_READ)) {
            result.push(client);
		}
		++it;
	} 
	return result;
}

/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
			Response
::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/

void	KqueueMultiplexer::add(IResponse* response)
{
	Responses.push_back(response);
	AddOrDeleteEvent(response->getSocketFd(), EVFILT_WRITE, EV_ADD);
}

void	KqueueMultiplexer::remove(IResponse* response)
{
	std::list<IResponse*>::iterator pos = std::find(Responses.begin(),
		Responses.end(), response);
		
	if (pos != Responses.end())
	{
		Responses.erase(pos);
		AddOrDeleteEvent(response->getSocketFd(), EVFILT_WRITE, EV_DELETE);
	}
}

std::queue<IResponse *> KqueueMultiplexer::getReadyResponses() const
{
	std::queue<IResponse *> result;

    if (ReadyEvents == 0)
        return result;

	std::list<IResponse* >::const_iterator it = Responses.begin();
	while (it != Responses.end())
	{
        IResponse *res = *it;
		if (IsEventSet(res->getSocketFd(), EVFILT_WRITE))
            result.push(res);
		++it;
	} 
	return result;
}

/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
			CGI
::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/


void  KqueueMultiplexer::add(IProxyRequest*  request, IMultiplexer::mod_t mod)
{
	std::cout << "Adding proxy request :: " << request->getOutputFd() << " | " << request->getSocketFd() << std::endl;
	ProxyRequests.push_back(request);
	if (mod == IMultiplexer::WRITE)
	{
		// std::cout << "Adding Proxy Request Sock For Write :: " <<  request->getOutputFd() << std::endl;
		AddOrDeleteEvent(request->getOutputFd(), EVFILT_WRITE, EV_ADD);
		return ;
	}
	// std::cout << "Adding Proxy Request Sock For Read :: " <<  request->getSocketFd() << std::endl;
	if ( request->getSocketFd() != -1 )
		AddOrDeleteEvent(request->getSocketFd(), EVFILT_READ, EV_ADD);
}

void  KqueueMultiplexer::remove(IProxyRequest*  request, IMultiplexer::mod_t mod)
{
	std::list<IProxyRequest *>::iterator pos = std::find(ProxyRequests.begin(),
		ProxyRequests.end(), request);
	
	if (pos == ProxyRequests.end())
		return ;

	if (mod == IMultiplexer::READ)
	{
		if (request->getSocketFd() != -1)
		{
			// std::cout << "Removing Proxy Request : "<<  request->getSocketFd() << " | " << request->getOutputFd() << std::endl;
			AddOrDeleteEvent(request->getSocketFd(), EVFILT_READ, EV_DELETE);
		}
		
		if ( ! IsEventSet(request->getOutputFd(), EVFILT_WRITE) )
		{
			std::cout << "Removing Request Object fd -> " << request->getSocketFd() << " | " << request->getOutputFd() << std::endl;
			ProxyRequests.erase(pos);
		}
		return ;
	}
	// std::cout << "Removing Proxy Request Sock FROM WRITE :: " <<  request->getOutputFd() << std::endl;
	AddOrDeleteEvent(request->getOutputFd(), EVFILT_WRITE, EV_DELETE);
	
	if (request->getSocketFd() != -1 || ! IsEventSet(request->getSocketFd(), EVFILT_READ) )
	{
		std::cout << "Removing Request Object fd -> " << request->getSocketFd() << " | " << request->getOutputFd() << std::endl;
		ProxyRequests.erase(pos);
	}
}

/*****************************      Proxy Response   *******************************/

void  KqueueMultiplexer::add(IProxyResponse* res, IMultiplexer::mod_t mod)
{
	std::cout << "Adding proxy response :: " << res->getInputFd() << " | " << res->getSocketFd() << std::endl;
	std::list<IProxyResponse*>::iterator pos = std::find(ProxyResponses.begin(), ProxyResponses.end(), res);
	if (pos == ProxyResponses.end())
		ProxyResponses.push_back(res);
	if (mod == IMultiplexer::READ)
	{
		// std::cout << "Adding Proxy Reponse Sock For Read :: " <<  res->getInputFd() << std::endl;
		AddOrDeleteEvent(res->getInputFd(), EVFILT_READ, EV_ADD);
		set[res->getInputFd()].read = 1;
		return ;
	}
	// std::cout << "Adding Proxy Reponse Sock For Write :: " <<  res->getSocketFd() << std::endl;
	AddOrDeleteEvent(res->getSocketFd(), EVFILT_WRITE, EV_ADD);
	set[res->getInputFd()].write = 1;
}

void  		KqueueMultiplexer::remove(IProxyResponse* response, IMultiplexer::mod_t mod)
{
	std::list<IProxyResponse *>::iterator pos = std::find(ProxyResponses.begin(),
		ProxyResponses.end(), response);

	if (pos == ProxyResponses.end())
		return ;

	if (mod == IMultiplexer::READ)
	{
		// std::cout << "Removing Proxy Response Sock FROM READ :: " <<  response->getInputFd() << std::endl;
		AddOrDeleteEvent(response->getInputFd(), EVFILT_READ, EV_DELETE);
		Logger::info("Removing Response Object fd  (read) -> ")(response->getSocketFd())(" , ")(response->getInputFd()).flush();
		set[response->getInputFd()].read = 0;

		if ( set[response->getInputFd()].write == 0 || ! IsEventSet(response->getSocketFd(), EVFILT_WRITE) )// wset[res->getSocketFd()]
		{
			std::cout << "Removing Response Object fd -> " << response->getSocketFd() << response->getInputFd() <<  std::endl;
			ProxyResponses.erase(pos);
			Logger::info("proxy responses size: ")(ProxyResponses.size()).flush();
		}
		return ;
	}
	// std::cout << "Removing Proxy Response Sock FROM WRITE :: "<<  response->getSocketFd() << std::endl;
	AddOrDeleteEvent(response->getSocketFd(), EVFILT_WRITE, EV_DELETE);
	Logger::info("Removing Response Object fd  (write) -> ")(response->getSocketFd())(" , ")(response->getInputFd()).flush();
	set[response->getInputFd()].write = 0;
	
	if (set[response->getInputFd()].read == 0 || ! IsEventSet(response->getInputFd(), EVFILT_READ) )
	{
		std::cout << "Removing Response Object fd -> " << response->getInputFd() << " | " << response->getSocketFd() << std::endl;
		ProxyResponses.erase(pos);
		Logger::info("proxy responses size: ")(ProxyResponses.size()).flush();
	}
}


/*****************************      Get Ready Proxy Requests   *******************************/

std::queue<IProxyRequest* >  		KqueueMultiplexer::getReadyForReadingProxyRequests() const
{
	std::queue<IProxyRequest* > ret;

	if (ReadyEvents == 0)
		return (ret);

	std::list<IProxyRequest *>::const_iterator it = ProxyRequests.begin();
	while (it != ProxyRequests.end())
	{
	    IProxyRequest *req = *it;
		if (req->getSocketFd() != -1 && IsEventSet(req->getSocketFd(), EVFILT_READ))
			ret.push(req);
		++it;	
	}
	return ret;
}

std::queue<IProxyRequest* >  KqueueMultiplexer::getReadyForWritingProxyRequests() const
{
	std::queue<IProxyRequest* > ret;

	if (ReadyEvents == 0)
		return (ret);

	std::list<IProxyRequest *>::const_iterator it = ProxyRequests.begin();
	while (it != ProxyRequests.end())
	{
	    IProxyRequest *request = *it;
    	if (IsEventSet(request->getOutputFd(), EVFILT_WRITE))
			ret.push(request);
		++it;	
	}
	return ret;
}


/*****************************      Get Ready Proxy Responses    *******************************/

std::queue<IProxyResponse* >  KqueueMultiplexer::getReadyForReadingProxyResponses() const 
{
	std::queue<IProxyResponse* > ret;

	if (ReadyEvents == 0)
		return (ret);
	
	Logger::info("proxy responses size: ")(ProxyResponses.size()).flush();

	std::list<IProxyResponse *>::const_iterator it = ProxyResponses.begin();
	while (it != ProxyResponses.end())
	{
	    IProxyResponse *response = *it;
       	if (IsEventSet(response->getInputFd(), EVFILT_READ))
            ret.push(response);
		++it;
	}
	return ret;
}

std::queue<IProxyResponse* >  KqueueMultiplexer::getReadyForWritingProxyResponses() const
{
	std::queue<IProxyResponse* > ret;

	if (ReadyEvents == 0)
		return (ret);

	std::list<IProxyResponse *>::const_iterator it = ProxyResponses.begin();
	while (it != ProxyResponses.end())
	{
	    IProxyResponse *response = *it;
		if (IsEventSet(response->getSocketFd(), EVFILT_WRITE))
			ret.push(response);
		++it;
	}
	return ret;
}

/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
			Upload
::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/

void KqueueMultiplexer::add(IUpload *upload)
{
	Uploads.push_back(upload);
	AddOrDeleteEvent(upload->getSocketFd(), EVFILT_READ, EV_ADD);
}

void KqueueMultiplexer::remove(IUpload *upload)
{
	std::cout << "Removing Upload event ....\n";
	std::list<IUpload *>::iterator pos = std::find(Uploads.begin(), Uploads.end(), upload);
	
	if (pos != Uploads.end())
	{
		Uploads.erase(pos);
		AddOrDeleteEvent(upload->getSocketFd(), EVFILT_WRITE, EV_DELETE);
	}
}

std::queue<IUpload *>	KqueueMultiplexer::getReadyUploads() const
{
	std::queue<IUpload *> ret;

	std::list<IUpload *>::const_iterator it = Uploads.begin();
	while (it != Uploads.end())
	{
		IUpload *upload = *it;
		if (IsEventSet(upload->getSocketFd(), EVFILT_READ))
			ret.push(upload);
		++it;
	}
	return ret;
	
}

/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
			Waiting For an Event
::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/

void	KqueueMultiplexer::wait(unsigned long int time)
{
	struct timespec timeout;

  	timeout.tv_sec = static_cast<long>(time / 1000000);
  	timeout.tv_nsec = static_cast<long>(time % 1000000);

	ReadyEvents = kevent(Kq, NULL, 0, Events.data(), Events.size(), &timeout);
	if (ReadyEvents < 0)
	{
		perror("Multiplexer Failed :");
		exit(EXIT_FAILURE);
	}
	if (ReadyEvents == 0) return ;
	readyEventsMap.clear();
	for (int i = 0; i < ReadyEvents; ++i)
	{
		int fd = Events[i].ident;
		readyEventsMap[fd] = Events[i];

		// Events[i]
	}
}

// void		KqueueMultiplexer::setupReadAndWriteSets()
// {
// 	for (size_t i = 0; i < readSet.size(); ++i)
// 		readSet[i] = true;
// 	for (size_t i = 0; i < writeSet.size(); ++i)
// 		readSet[i] = true;
// }

 bool KqueueMultiplexer::ready() const {
	return ReadyEvents > 0;
 }

bool	KqueueMultiplexer::IsEventSet(unsigned int fd, short filter) const
{
	std::map<int, struct kevent>::const_iterator pos = readyEventsMap.find(fd);
	if (pos != readyEventsMap.end())
	{
		if (filter == pos->second.filter || filter == (EVFILT_READ | EVFILT_WRITE) )
			return true ;
	}
	return false;
}

bool 	KqueueMultiplexer::IsSet(unsigned int fd, short filter) const
{
	std::vector<struct kevent>::const_iterator it = Events.begin();

	while  (it != Events.end())
	{
		if (fd == it->ident && ( filter == it->filter || filter == (EVFILT_READ | EVFILT_WRITE) ))
			return true;
		++it;
	}
	return false;
}

void	KqueueMultiplexer::AddOrDeleteEvent(unsigned int fd, short filter, short flag)
{
	if (flag == EV_DELETE && !IsSet(fd, filter)) {
        std::cout << "Sock not monitored :: " << fd << std::endl;
        return;
    }
	
	EV_SET(&event, fd, filter, flag, 0, 0, NULL);
	if (kevent(Kq, &event, 1, NULL, 0, NULL) == -1)
	{
		std::cout << "Failed To Remove Sock :: " << fd << std::endl;
		perror("kevent: Failed ");
		exit(EXIT_FAILURE);
	}

	if (flag == EV_DELETE)
	{
		// std::cout << "Looking For :: " << fd << " | Filter :: " << filter << std::endl;
		std::vector<struct kevent>::iterator it = Events.begin();

		while  ( it != Events.end() )
		{
			if (fd == it->ident && filter == it->filter)
			{
				Events.erase(it);
				return ;
			}
			++it;
		}
	}
	if (flag == EV_ADD) Events.push_back(event);
	// std::cout << "vector :: " << Events.size() << std::endl;
	// std::cout << "map :: " << readyEventsMap.size() << std::endl;
}


