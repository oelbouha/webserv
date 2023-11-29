/*	                     __          _
 *	   __  ___________ _/ /___ ___  (_)
 *	  / / / / ___/ __ `/ / __ `__ \/ /
 *	 / /_/ (__  ) /_/ / / / / / / / / 
 *	 \__, /____/\__,_/_/_/ /_/ /_/_/ 
 *	/____/	User: Youssef Salmi
 *			File: ClientSocket.cpp 
 */

#include "ClientSocket.hpp"

ClientSocket::ClientSocket(int aFileDes) :
mID(aFileDes)
{}

ClientSocket::ClientSocket( const ClientSocket& aClientSocket ) :
mID(aClientSocket.mID)
{}

ClientSocket::~ClientSocket()
{}

ClientSocket&	ClientSocket::operator=( const ClientSocket& aClientSocket )
{
	if (this != &aClientSocket)
	{}
	return (*this);
}

int		ClientSocket::write( const char* aBuffer, int aSize )
{
	int	r = ::write(mID, aBuffer, aSize);

	std::cout << "r = " << r << std::endl << std::flush;
	if (r < 0)
		throw SocketException(std::string("Couldn't write to socket: ") + strerror(errno));
	return (r);
}

std::string		ClientSocket::read(unsigned int aMaxSize)
{
	char	buffer[aMaxSize + 1];

	int	r = ::read(mID, buffer, aMaxSize);

	if (r < 0)
		throw SocketException("Empty socket buffer");

	return (std::string(buffer, r));
}

std::string		ClientSocket::readHeaderOnly()
{
	std::string	ret;
	size_t		pos;

	pos = mBuffer.find("\r\n\r\n");

	try
	{
		while (pos == std::string::npos)
		{
			mBuffer += read(512);
			pos = mBuffer.find("\r\n\r\n");
		}
	}
	catch(const SocketException& e)
	{
		throw SocketException("Incomplete request header");
	}

	ret = mBuffer.substr(0, pos);
	mBuffer = mBuffer.substr(pos + 4);
	return (ret);
}

std::string		ClientSocket::readAll()
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

void	ClientSocket::dump()
{
	std::string	s;
	
	while (true)
	{
		try{
			s = read(1024);
			for (std::string::iterator it = s.begin(); it != s.end(); ++it)
			{
				if ( *it != 10 && (*it < 32 || *it > 126))
					std::cout << "▒";
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
	std::cout << std::endl << std::flush;
}


void            ClientSocket::close()
{
	::close(mID);
}

void            ClientSocket::setNonBlocking()
{
	if (fcntl(mID, F_SETFL, O_NONBLOCK, O_CLOEXEC) < 0)
		throw SocketException("Can't switch socket to non-blocking mode");
}
