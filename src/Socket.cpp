/*	                     __          _
 *	   __  ___________ _/ /___ ___  (_)
 *	  / / / / ___/ __ `/ / __ `__ \/ /
 *	 / /_/ (__  ) /_/ / / / / / / / / 
 *	 \__, /____/\__,_/_/_/ /_/ /_/_/ 
 *	/____/	User: Youssef Salmi
 *			File: Socket.cpp 
 */

#include "Socket.hpp"

int	Socket::sBackLog = 5;

Socket::Socket()
{
	if ((mID = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		throw SocketException("Can't create socket");
}

Socket::Socket( const Socket& s ) : mID(s.mID)
{}

Socket::Socket( int aID ) : mID(aID)
{}

Socket::Socket(unsigned long int aIP, unsigned int aPORT)
{
	if ((mID = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		throw SocketException("Can't create socket");
	bind(aIP, aPORT);
}

Socket::~Socket()
{}

Socket&	Socket::operator=( const Socket& s )
{
	if (this != &s)
		mID = s.mID;
	return (*this);
}

int		Socket::getID() const
{
	return (mID);
}

void	Socket::bind(unsigned int aIP, unsigned int aPORT)
{
	struct sockaddr_in	addr;
	std::size_t			addr_len = sizeof(addr);

	int	opt = 0;
	// make address reusable
	if (::setsockopt(mID, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
		throw SocketException(std::string("Can't reuse addr/port ") + strerror(errno));
	opt = 1;
	// make port reusable
	if (::setsockopt(mID, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt)) < 0)
		throw SocketException(std::string("Can't reuse addr/port ") + strerror(errno));

	bzero(&addr, addr_len);
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htons(aIP);
	addr.sin_port = htons(aPORT);

	if (::bind(mID, (struct sockaddr *)&addr, addr_len) < 0)
	{
		if (errno == EADDRNOTAVAIL)
			std::cerr << "The specified address is not available from the local machine." << std::endl << std::flush;
		throw SocketException("Can't bind socket to " + std::to_string(aIP) + ":" + std::to_string(aPORT) + " " + strerror(errno));
	}
}

void	Socket::listen()
{
	if (::listen(mID, Socket::sBackLog) < 0)
		throw SocketException("Socket is not listenning");
}

ISocket*	Socket::accept()
{
	struct sockaddr_in	addr;
	std::size_t			addr_len = sizeof(addr);

	int	id = ::accept(mID, (struct sockaddr *)&addr, (socklen_t*)&addr_len);

	// std::cout << utils::ip(htonl(addr.sin_addr.s_addr)) << ":" << addr.sin_port << std::endl << std::flush;
	// logger::inform << utils::ip(htonl(addr.sin_addr.s_addr)) << ":" << addr.sin_port << std::endl << std::flush;
	// logger::info_(
	// 	utils::ip(htonl(addr.sin_addr.s_addr))
	// 	+ ":" + std::to_string(addr.sin_port) + "\n"
	// );
	return (new Socket(id));
}

void	Socket::close()
{
	::close(mID);
}

void	Socket::setNonBlocking()
{
	if (fcntl(mID, F_SETFL, O_NONBLOCK, O_CLOEXEC) < 0)
		throw SocketException("Can't switch socket to non-blocking mode");
}

int		Socket::write(const char* aBuffer, int aSize)
{
	return (::write(mID, aBuffer, aSize));
}

std::string		Socket::read(unsigned int aMaxSize)
{
	char	buffer[aMaxSize + 1];

	int	r = ::read(mID, buffer, aMaxSize);

	if (r < 0)
		throw SocketException("Empty socket buffer");
	
	// buffer[r] = 0; 
	return (std::string(buffer, r));
}

std::string		Socket::readAll()
{
	std::string	ret;
	char		buffer[1024];
	int			r;

	while ((r = ::read(mID, buffer, 1023)) > 0)
	{
		buffer[r] = 0;
		ret += buffer;
	}
	return (ret);
}

#include <unistd.h>
void	Socket::dump()
{
	std::string	s;
	
	while (true)
	{
		try{
			s = read(1024);
			for (std::string::iterator it = s.begin(); it != s.end(); ++it)
			{
				if ( *it != 10 && (*it < 32 || *it > 126))
					std::cout << '[' << (int)*it << ']';
					// std::cout << "▒";
				else
					std::cout << *it;
			}
			std::cout << std::flush;
		}
		catch(...)
		{
			break ;
		}
	}
	std::cout << "=========================" << std::endl << std::flush;
}