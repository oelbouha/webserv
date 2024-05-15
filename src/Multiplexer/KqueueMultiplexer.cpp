/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   KqueueMultiplexer.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelbouha <oelbouha@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 15:10:02 by oelbouha          #+#    #+#             */
/*   Updated: 2024/05/14 14:39:05 by oelbouha         ###   ########.fr       */
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
	{
		Logger::error("Kevent Failed To Watch Events").flush();
		std::exit (1);
	}
}

/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
			Server																	 
::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/

void	KqueueMultiplexer::add(IServerSocket* server)
{
	Servers[server->getSocketFd()] = server;
	addEventToQueue(server->getSocketFd(), EVFILT_READ);
}

void	KqueueMultiplexer::remove(IServerSocket* server)
{
	std::map<int, IServerSocket*>::iterator pos = Servers.find(server->getSocketFd());

	if (pos != Servers.end())
	{
		deleteEventFromQueue(server->getSocketFd(), EVFILT_READ);
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
	addEventToQueue(client->getSocketFd(), EVFILT_READ);
}

void	KqueueMultiplexer::remove(IClient* client)
{
	std::map<int, IClient*>::iterator pos = Clients.find(client->getSocketFd());

	if (pos != Clients.end())
	{
		Clients.erase(pos);
		deleteEventFromQueue(client->getSocketFd(), EVFILT_READ);
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
	addEventToQueue(res->getSocketFd(), EVFILT_WRITE);
}

void	KqueueMultiplexer::remove(IResponse* res)
{
	std::map<int, IResponse*>::iterator pos = Responses.find(res->getSocketFd());
		
	if (pos != Responses.end())
	{
		Responses.erase(pos);
		deleteEventFromQueue(res->getSocketFd(), EVFILT_WRITE);
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
			ADD Or Remove Proxyrequest
::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/

void  KqueueMultiplexer::add(IProxyRequest*  request, IMultiplexer::mod_t mod)
{
	ProxyRequests[request->getOutputFd()] = request;
	if (mod == IMultiplexer::WRITE)
	{
		addEventToQueue(request->getOutputFd(), EVFILT_WRITE);
		return ;
	}
	if ( request->getSocketFd() != -1 )
		addEventToQueue(request->getSocketFd(), EVFILT_READ);
}

void  KqueueMultiplexer::remove(IProxyRequest*  request, IMultiplexer::mod_t mod)
{
	std::map<int, IProxyRequest *>::iterator pos = ProxyRequests.find(request->getOutputFd());
	
	if (pos == ProxyRequests.end())
		return ;

	if (mod == IMultiplexer::READ)
	{
		if (request->getSocketFd() != -1)
			deleteEventFromQueue(request->getSocketFd(), EVFILT_READ);
		
		if (! isEventExists(request->getOutputFd(), EVFILT_WRITE))
			ProxyRequests.erase(pos);
		return ;
	}

	deleteEventFromQueue(request->getOutputFd(), EVFILT_WRITE);	
	if (! isEventExists(request->getSocketFd(), EVFILT_READ))
		ProxyRequests.erase(pos);
}


/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
			ADD Or Remove ProxyResponse
::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/

void  KqueueMultiplexer::add(IProxyResponse* res, IMultiplexer::mod_t mod)
{
	ProxyResponses[res->getSocketFd()] = res;
	if (mod == IMultiplexer::READ)
	{
		addEventToQueue(res->getInputFd(), EVFILT_READ);
		return ;
	}
	addEventToQueue(res->getSocketFd(), EVFILT_WRITE);
}

void  		KqueueMultiplexer::remove(IProxyResponse* response, IMultiplexer::mod_t mod)
{
	std::map<int, IProxyResponse *>::iterator pos = ProxyResponses.find(response->getSocketFd());

	if (pos == ProxyResponses.end())
		return ;

	if (mod == IMultiplexer::READ)
	{
		deleteEventFromQueue(response->getInputFd(), EVFILT_READ);
		if (! isEventExists(response->getSocketFd(), EVFILT_WRITE) )
			ProxyResponses.erase(pos);
		return ;
	}
	deleteEventFromQueue(response->getSocketFd(), EVFILT_WRITE);
	if (! isEventExists(response->getInputFd(), EVFILT_READ) )
		ProxyResponses.erase(pos);
}

/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
			Get Ready Proxy Requests
::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/

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

/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
			Get Ready Proxy Responses
::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/

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
	addEventToQueue(upload->getSocketFd(), EVFILT_READ);
}

void KqueueMultiplexer::remove(IUpload *upload)
{
	std::map<int, IUpload *>::iterator pos = Uploads.find(upload->getSocketFd());
	
	if (pos != Uploads.end())
	{
		Uploads.erase(pos);
		deleteEventFromQueue(upload->getSocketFd(), EVFILT_READ);
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
	if (ReadyEvents == 0) return ;

	if (ReadyEvents < 0) {
		Logger::error("Kevent Failed To Watch Events").flush();
		std::exit (1);
	}

	readyEventsMap.clear();
	for (int i = 0; i < ReadyEvents; ++i)
	{
		int fd = Events[i].ident;
		readyEventsMap[fd] = Events[i];
	}
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

bool 	KqueueMultiplexer::isEventExists(unsigned int fd, short filter) const
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

void	KqueueMultiplexer::removeEventFromEventsSet(unsigned int fd, short filter)
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

void	KqueueMultiplexer::addEventToQueue(unsigned int fd, short filter)
{
	struct kevent event;

	Events.push_back(event);
	
	EV_SET(&event, fd, filter, EV_ADD, 0, 0, NULL);
	if (kevent(Kq, &event, 1, NULL, 0, NULL) < 0)
	{
		Logger::error("Kevent Failed To set The Event").flush();
		std::exit (1);
	}
}

void	KqueueMultiplexer::deleteEventFromQueue(unsigned int fd, short filter)
{
	struct kevent event;

	if (! isEventExists(fd, filter))
		return ;

	removeEventFromEventsSet(fd, filter);

	EV_SET(&event, fd, filter, EV_DELETE, 0, 0, NULL);
	if (kevent(Kq, &event, 1, NULL, 0, NULL) < 0)
	{
		Logger::error("Kevent Failed To set The Event").flush();
		std::exit (1);
	}
}

bool KqueueMultiplexer::ready() const {
	return ReadyEvents > 0;
}

