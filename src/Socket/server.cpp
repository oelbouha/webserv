/*	                     __          _
 *	   __  ___________ _/ /___ ___  (_)
 *	  / / / / ___/ __ `/ / __ `__ \/ /
 *	 / /_/ (__  ) /_/ / / / / / / / / 
 *	 \__, /____/\__,_/_/_/ /_/ /_/_/ 
 *	/____/	User: Youssef Salmi
 *			File: main.cpp 
 */

#include <iostream>
#include <string>
#include <cstring>

#include <unistd.h>

#include <sys/types.h>
#include <sys/socket.h>

#include "ServerSocket.hpp"
#include "ClientSocket.hpp"

#include <signal.h>
#include <sys/select.h>


int main()
{
	signal(SIGPIPE, SIG_IGN);

	ServerSocket	server(0, 7000);

	server.bind();
	server.listen();

	IClientSocket*	client = server.accept();

	client->setNonBlocking();

	std::cout << client->readAll() << std::endl;

	sleep(1);
	// client->readAll();
	for (int i = 0; i < 20000; ++i)
	{
        struct timeval timeout;
        size_t  timeout_us = 1000000;
        timeout.tv_sec = static_cast<long>(timeout_us / 1000000);
        timeout.tv_usec = static_cast<long>(timeout_us % 1000000);

		usleep(1000);

		fd_set  wr;
		fd_set  rd;
        fd_set  exp;

		FD_ZERO(&wr);
		FD_SET(client->getSocketFd(), &wr);
		
		FD_ZERO(&rd);
		FD_SET(client->getSocketFd(), &rd);

		FD_ZERO(&exp);
		FD_SET(client->getSocketFd(), &exp);

		int r = ::select(10, &rd, &wr, &exp, &timeout);

		if (FD_ISSET(client->getSocketFd(), &rd))
		{
			std::cout << "ready to read\n" << std::flush;
			// client->readAll();
		}

		if (FD_ISSET(client->getSocketFd(), &wr))
        {
            std::cout << "ready to write\n" << std::flush;
			client->write("hello from server\n");
        }

        
        if (FD_ISSET(client->getSocketFd(), &exp))
        {
            std::cout << "exption\n" << std::flush;
        }

		std::cout << i << std::endl;
	}
	
	std::cout << "end." << std::endl;

	return (0);
}
