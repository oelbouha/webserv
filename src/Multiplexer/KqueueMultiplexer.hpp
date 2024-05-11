/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   KqueueMultiplexer.hpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelbouha <oelbouha@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/04 14:52:03 by oelbouha          #+#    #+#             */
/*   Updated: 2024/05/11 15:38:31 by oelbouha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef KQUEUEMULTIPLEXER_HPP
#define KQUEUEMULTIPLEXER_HPP

#include <map>
#include <queue>
#include <bitset>
#include <list>
#include <vector>

#include <iostream>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/event.h>
#include <unistd.h>

#include "../Route/Upload.hpp"
#include "IMultiplexer.hpp"


#include "Logger.hpp"

class KqueueMultiplexer : public IMultiplexer
{
	std::map<int , IServerSocket *> 			Servers;
	std::map<int , IClient *> 					Clients;
	std::map<int , IResponse *> 				Responses;
	std::map<int , IProxyRequest *> 			ProxyRequests;
	std::map<int , IProxyResponse *>			ProxyResponses;
	std::map<int , IUpload *>					Uploads;

public:
	KqueueMultiplexer();
	~KqueueMultiplexer();

	std::queue<IServerSocket *>	 	getReadyServerSockets() const;
	std::queue<IClient *>			getReadyClients() const;
	std::queue<IResponse *>			getReadyResponses() const;
	std::queue<IUpload *>			getReadyUploads() const;


 	void 		add(IUpload *upload);
	void 		add(IClient *aClient) ;
	void 		add(IResponse *response) ;
	void 		add(IServerSocket *server) ;
	void  		add(IProxyRequest*  request, mod_t mod);
	void  		add(IProxyResponse*  response, mod_t mod);

  	void 		remove(IUpload *upload);
	void 		remove(IClient *client) ;
	void 		remove(IResponse *response) ;
	void 		remove(IServerSocket *server) ;
	void  		remove(IProxyRequest*  request, mod_t mod);
	void  		remove(IProxyResponse*  response, mod_t mod);


	std::queue<IProxyRequest*>		getReadyForReadingProxyRequests() const;
	std::queue<IProxyRequest*>		getReadyForWritingProxyRequests() const;
	std::queue<IProxyResponse*>		getReadyForReadingProxyResponses() const;
	std::queue<IProxyResponse*>		getReadyForWritingProxyResponses() const;

	bool		ready() const ;
	void 		wait(unsigned long int timeout) ;
	bool		IsEventSet(unsigned int socketFd, short filter) const ;
	void		AddOrDeleteEvent(unsigned int socketFd, short filter, short flag);
	bool 		IsSet(unsigned int socketFd, short filter) const ;
	void		deleteEvent(unsigned int fd, short filter);
	
private:
	
	std::vector<struct kevent> 		Events;
	std::map<int, struct kevent>	readyEventsMap;
	int 							ReadyEvents;
	int								Kq;
};

#endif