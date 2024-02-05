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

#include "ClientSocket.hpp"

int main()
{
	ClientSocket	sock((::socket(AF_INET, SOCK_STREAM, 0)));

	struct sockaddr_in addr;
  	std::size_t addr_len = sizeof(addr);

	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(127 << 24 | 1);
	addr.sin_port = htons(8000);
	::connect(sock.getSocketFd(), (sockaddr*)&addr, addr_len);

	std::string	resp = "GET /pages HTTP/1.1\r\n";
	resp += "host: localhost:8000\r\n";
	// resp += "content-length: 11\r\n";
	resp += "\r\n";

    std::cout << resp;

	sock.setNonBlocking();
	
	sock.write(resp);

    sleep(1);

	std::string	s = sock.readAll();

	for (int i = 0; i < s.length(); ++i)
	{
		if (s[i] < 32 && s[i] != 10)
			std::cout << '[' << (int)s[i] << ']';
		else
			std::cout << s[i];
	}
	std::cout << std::endl;


	return (0);
}
