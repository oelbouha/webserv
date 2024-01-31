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
	addr.sin_port = htons(7000);
	::connect(sock.getSocketFd(), (sockaddr*)&addr, addr_len);

	std::string	resp = "POST / HTTP/1.1\r\n";
	resp += "host: localhost\r\n";
	// resp += "content-length: 11\r\n";
	resp += "transfer-encoding: chunked\r\n";
	resp += "\r\n";
	resp += "8\r\nMozilla \r\n11\r\nDeveloper Network\r\n0\r\n\r\n";

    std::cout << resp;

	sock.setNonBlocking();
	
	sock.write(resp);

    // sleep(2);

	std::cout << sock.readAll() << std::endl;

	return (0);
}
