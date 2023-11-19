/*	                     __          _
 *	   __  ___________ _/ /___ ___  (_)
 *	  / / / / ___/ __ `/ / __ `__ \/ /
 *	 / /_/ (__  ) /_/ / / / / / / / / 
 *	 \__, /____/\__,_/_/_/ /_/ /_/_/ 
 *	/____/	User: Youssef Salmi
 *			File: Socket.hpp 
 */

#pragma once
#ifndef SOCKET_HPP
#define SOCKET_HPP

#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netinet/in.h>

#include "ISocket.hpp"
#include "SocketException.hpp"
#include "Logger.hpp"
#include "Utils.hpp"

class Socket : public ISocket
{
	int		mID;

	static int	sBackLog;

	public:
		Socket();
		Socket( const Socket& s );
		Socket(int aID);
		Socket(unsigned long int aIP, unsigned int aPORT);
		~Socket();

		Socket&	operator=( const Socket& s );

		int			getID() const;
		void		setNonBlocking();

		void		bind(unsigned int aIP, unsigned int aPORT);
		void		listen();
		ISocket*	accept();
		void		close();


		int			write(const char* buffer, int size);
		std::string	read(unsigned int aMaxSize);
		std::string	readAll();

		void	dump();

};
#endif
