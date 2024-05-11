/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   KqueueMultiplexer.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelbouha <oelbouha@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 15:10:02 by oelbouha          #+#    #+#             */
/*   Updated: 2024/05/11 12:21:38 by oelbouha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "KqueueMultiplexer.hpp"

KqueueMultiplexer::~KqueueMultiplexer(){
	close (Kq);
}

KqueueMultiplexer::KqueueMultiplexer()
{
    Kq = kqueue();
	if (Kq < 0)
		throw MultiplexerException("Failed To Create Kqueue");
}

/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
			Server																	 
::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/

void	KqueueMultiplexer::add(IServerSocket* server)
{
	Servers[server->getSocketFd()] = server;
	AddOrDeleteEvent(server->getSocketFd(), EVFILT_READ, EV_ADD);
}

void	KqueueMultiplexer::remove(IServerSocket* server)
{
	std::map<int, IServerSocket*>::iterator pos = Servers.find(server->getSocketFd());

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

    std::map<int, IServerSocket *>::const_iterator it = Servers.begin();
	while (it != Servers.end())
	{
		if (IsEventSet(it->first, EVFILT_READ))
            ret.push(it->second);
		++it;
	} 
	return ret;
}

/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
			Client
::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/

void	KqueueMultiplexer::add(IClient* client)
{
	Clients[client->getSocketFd()] = client;
	AddOrDeleteEvent(client->getSocketFd(), EVFILT_READ, EV_ADD);
}

void	KqueueMultiplexer::remove(IClient* client)
{
	std::map<int, IClient*>::iterator pos = Clients.find(client->getSocketFd());

	if (pos != Clients.end())
	{
		Clients.erase(pos);
		AddOrDeleteEvent(client->getSocketFd(), EVFILT_READ, EV_DELETE);
	}
}

std::queue<IClient *> KqueueMultiplexer::getReadyClients() const
{
	std::queue<IClient *> result;

	if (ReadyEvents == 0)
        return result;
	
	std::map<int, IClient*>::const_iterator it = Clients.begin();
	while (it != Clients.end())
	{
		if (IsEventSet(it->first, EVFILT_READ))
            result.push(it->second);
		++it;
	} 
	return result;
}

/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
			Response
::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/

void	KqueueMultiplexer::add(IResponse* res)
{
	Responses[res->getSocketFd()] = res;
	AddOrDeleteEvent(res->getSocketFd(), EVFILT_WRITE, EV_ADD);
}

void	KqueueMultiplexer::remove(IResponse* res)
{
	std::map<int, IResponse*>::iterator pos = Responses.find(res->getSocketFd());
		
	if (pos != Responses.end())
	{
		Responses.erase(pos);
		AddOrDeleteEvent(res->getSocketFd(), EVFILT_WRITE, EV_DELETE);
	}
}

std::queue<IResponse *> KqueueMultiplexer::getReadyResponses() const
{
	std::queue<IResponse *> result;

    if (ReadyEvents == 0)
        return result;

	std::map<int, IResponse* >::const_iterator it = Responses.begin();
	while (it != Responses.end())
	{
		if (IsEventSet(it->first, EVFILT_WRITE))
            result.push(it->second);
		++it;
	} 
	return result;
}

/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
			CGI
::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/


void  KqueueMultiplexer::add(IProxyRequest*  request, IMultiplexer::mod_t mod)
{
	ProxyRequests[request->getOutputFd()] = request;
	if (mod == IMultiplexer::WRITE)
	{
		AddOrDeleteEvent(request->getOutputFd(), EVFILT_WRITE, EV_ADD);
		return ;
	}
	if ( request->getSocketFd() != -1 )
		AddOrDeleteEvent(request->getSocketFd(), EVFILT_READ, EV_ADD);
}

void  KqueueMultiplexer::remove(IProxyRequest*  request, IMultiplexer::mod_t mod)
{
	std::map<int, IProxyRequest *>::iterator pos = ProxyRequests.find(request->getOutputFd());
	
	if (pos == ProxyRequests.end())
		return ;

	if (mod == IMultiplexer::READ)
	{
		if (request->getSocketFd() != -1)
			AddOrDeleteEvent(request->getSocketFd(), EVFILT_READ, EV_DELETE);
		
		if ( ! IsSet(request->getOutputFd(), EVFILT_WRITE) )
			ProxyRequests.erase(pos);
		return ;
	}

	AddOrDeleteEvent(request->getOutputFd(), EVFILT_WRITE, EV_DELETE);	
	if (request->getSocketFd() != -1 || ! IsSet(request->getSocketFd(), EVFILT_READ))
		ProxyRequests.erase(pos);
}

/*****************************      Proxy Response   *******************************/

void  KqueueMultiplexer::add(IProxyResponse* res, IMultiplexer::mod_t mod)
{
	ProxyResponses[res->getSocketFd()] = res;
	if (mod == IMultiplexer::READ)
	{
		AddOrDeleteEvent(res->getInputFd(), EVFILT_READ, EV_ADD);
		return ;
	}
	AddOrDeleteEvent(res->getSocketFd(), EVFILT_WRITE, EV_ADD);
}

void  		KqueueMultiplexer::remove(IProxyResponse* response, IMultiplexer::mod_t mod)
{
	std::map<int, IProxyResponse *>::iterator pos = ProxyResponses.find(response->getSocketFd());

	if (pos == ProxyResponses.end())
		return ;

	if (mod == IMultiplexer::READ)
	{
		AddOrDeleteEvent(response->getInputFd(), EVFILT_READ, EV_DELETE);
		// Logger::info("Removing Response Object fd  (read) -> ")(response->getSocketFd())(" , ")(response->getInputFd()).flush();

		if (! IsSet(response->getSocketFd(), EVFILT_WRITE) )
		{
			// std::cout << "Removing Response Object fd -> " << response->getSocketFd() << response->getInputFd() <<  std::endl;
			ProxyResponses.erase(pos);
			// Logger::info("proxy responses size: ")(ProxyResponses.size()).flush();
		}
		return ;
	}
	
	AddOrDeleteEvent(response->getSocketFd(), EVFILT_WRITE, EV_DELETE);
	// Logger::info("Removing Response Object fd  (write) -> ")(response->getSocketFd())(" , ")(response->getInputFd()).flush();
	
	if (! IsSet(response->getInputFd(), EVFILT_READ) )
	{
		// std::cout << "Removing Response Object fd -> " << response->getInputFd() << " | " << response->getSocketFd() << std::endl;
		ProxyResponses.erase(pos);
		// Logger::info("proxy responses size: ")(ProxyResponses.size()).flush();
	}
}


/*****************************      Get Ready Proxy Requests   *******************************/

std::queue<IProxyRequest* >  		KqueueMultiplexer::getReadyForReadingProxyRequests() const
{
	std::queue<IProxyRequest* > ret;

	if (ReadyEvents == 0)
		return (ret);

	std::map<int, IProxyRequest *>::const_iterator it = ProxyRequests.begin();
	while (it != ProxyRequests.end())
	{
		if (it->second->getSocketFd() != -1 && IsEventSet(it->second->getSocketFd(), EVFILT_READ))
			ret.push(it->second);
		++it;
	}
	return ret;
}

std::queue<IProxyRequest* >  KqueueMultiplexer::getReadyForWritingProxyRequests() const
{
	std::queue<IProxyRequest* > ret;

	if (ReadyEvents == 0)
		return (ret);

	std::map<int, IProxyRequest *>::const_iterator it = ProxyRequests.begin();
	while (it != ProxyRequests.end())
	{
    	if (IsEventSet(it->first, EVFILT_WRITE))
			ret.push(it->second);
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

	std::map<int, IProxyResponse *>::const_iterator it = ProxyResponses.begin();
	while (it != ProxyResponses.end())
	{
       	if (IsEventSet(it->second->getInputFd(), EVFILT_READ))
            ret.push(it->second);
		++it;
	}
	return ret;
}

std::queue<IProxyResponse* >  KqueueMultiplexer::getReadyForWritingProxyResponses() const
{
	std::queue<IProxyResponse* > ret;

	if (ReadyEvents == 0)
		return (ret);

	std::map<int, IProxyResponse *>::const_iterator it = ProxyResponses.begin();
	while (it != ProxyResponses.end())
	{
		if (IsEventSet(it->first, EVFILT_WRITE))
			ret.push(it->second);
		++it;
	}
	return ret;
}

/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
			Upload
::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/

void KqueueMultiplexer::add(IUpload *upload)
{
	Uploads[upload->getSocketFd()] = upload;
	AddOrDeleteEvent(upload->getSocketFd(), EVFILT_READ, EV_ADD);
}

void KqueueMultiplexer::remove(IUpload *upload)
{
	std::map<int, IUpload *>::iterator pos = Uploads.find(upload->getSocketFd());
	
	if (pos != Uploads.end())
	{
		Uploads.erase(pos);
		AddOrDeleteEvent(upload->getSocketFd(), EVFILT_READ, EV_DELETE);
	}
}

std::queue<IUpload *>	KqueueMultiplexer::getReadyUploads() const
{
	std::queue<IUpload *> ret;

	std::map<int, IUpload *>::const_iterator it = Uploads.begin();
	while (it != Uploads.end())
	{
		if (IsEventSet(it->first, EVFILT_READ))
			ret.push(it->second);
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
	if (ReadyEvents < 0) throw MultiplexerException("Kevent Failed To Watch Events");
	if (ReadyEvents == 0) return ;
	
	readyEventsMap.clear();
	for (int i = 0; i < ReadyEvents; ++i)
	{
		int fd = Events[i].ident;
		readyEventsMap[fd] = Events[i];
	}
}

 bool KqueueMultiplexer::ready() const { return ReadyEvents > 0; }

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

void	KqueueMultiplexer::deleteEvent(unsigned int fd, short filter)
{
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

void	KqueueMultiplexer::AddOrDeleteEvent(unsigned int fd, short filter, short flag)
{
	struct kevent event;

	EV_SET(&event, fd, filter, flag, 0, 0, NULL);
	if (kevent(Kq, &event, 1, NULL, 0, NULL) < 0)
		throw MultiplexerException("Kevent Failed To set The Event");
	if (flag == EV_DELETE)
		deleteEvent(fd, filter);
	else if (flag == EV_ADD)
	{
		if (IsSet(fd, filter))
			deleteEvent(fd, filter);
		Events.push_back(event);
	}
}


