/*	                     __          _
 *	   __  ___________ _/ /___ ___  (_)
 *	  / / / / ___/ __ `/ / __ `__ \/ /
 *	 / /_/ (__  ) /_/ / / / / / / / / 
 *	 \__, /____/\__,_/_/_/ /_/ /_/_/ 
 *	/____/	User: Youssef Salmi
 *			File: RequestParser.cpp 
 */

#include "RequestParser.hpp"

RequestParser::RequestParser(IClientSocket& aClientSocket):
mClientSocket(aClientSocket)
{}

RequestParser::RequestParser( const RequestParser& aRequestParser ):
mClientSocket(aRequestParser.mClientSocket),
mRequest(aRequestParser.mRequest)
{}

RequestParser::~RequestParser()
{}

RequestParser&	RequestParser::operator=( const RequestParser& aRequestParser )
{
	if (this != &aRequestParser)
	{

	}
	return (*this);
}

void	RequestParser::parse()
{
try {

	std::string	raw;

	while (true)
	{
		raw += mClientSocket.read(1024);

		// if (raw)
	}

}
catch(const SocketException& e)
{(void)e;return;}
}
