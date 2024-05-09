/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelbouha <oelbouha@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/07 15:52:51 by oelbouha          #+#    #+#             */
/*   Updated: 2024/05/08 10:15:06 by oelbouha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <map>
#include <queue>
#include <bitset>
#include <list>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <iostream>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/event.h>
#include <unistd.h>


int kqueue()
{
	int fd = open("Makefile", O_RDONLY);
	if (fd < 0)
	{
		perror("fd: ");
		exit (0);
	}
	int kq = kqueue();
	std::cout << "here ...  " << fd << " | " << kq << std::endl;;
	if (kq < 0)
	{
		perror("kqueue: ");
		exit (0);
	}
	struct kevent event;
	EV_SET(&event, fd, EVFILT_READ, EV_ADD, 0, 0, NULL);
	if (kevent(kq, &event, 1, NULL, 0, NULL) == -1)
	{
		perror("kevent: Failed ");
		exit(EXIT_FAILURE);
	}
	while (1)
	{
		int ReadyEvents = kevent(kq, NULL, 0, &event, fd, NULL);
		std::cout << "Events:: " << ReadyEvents << std::endl;
		if (ReadyEvents < 0)
		{
			perror("Multiplexer Failed :");
			exit(EXIT_FAILURE);
		}
		if (ReadyEvents)
		{
			char buff[20000];
			int r = read(fd, buff, 5);
			std::cout << "r :: " << r << std::endl;
		}
	}
	return (0);
}

int select()
{
	int fd = open("testfile", O_RDWR);

	fcntl(fd, F_SETFL, O_NONBLOCK, O_CLOEXEC);

	fd_set readfd;

	FD_ZERO(&readfd);
	while (1)
	{
		FD_SET(fd, &readfd);
		int ReadyEvents = select(fd + 1, &readfd, NULL, NULL, NULL);
		std::cout << "ReadyEvents :: " << ReadyEvents << std::endl;
		if (ReadyEvents)
		{
			char buff[20000];
			int r = read(fd, buff, 5);

			std::cout << "Buff : " << r << std::endl;
			sleep(1);
		}
	}
}

int main()
{
	kqueue();
	return 0;
}