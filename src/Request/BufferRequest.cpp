/*	                     __          _
 *	   __  ___________ _/ /___ ___  (_)
 *	  / / / / ___/ __ `/ / __ `__ \/ /
 *	 / /_/ (__  ) /_/ / / / / / / / / 
 *	 \__, /____/\__,_/_/_/ /_/ /_/_/ 
 *	/____/	User: Youssef Salmi
 *			File: BufferRequest.cpp 
 */

#include "BufferRequest.hpp"

BufferRequest::BufferRequest(const Request& request, const std::string& body) :
	Request(request)
{
	mBody = body;
	Logger::debug ("buffer req cons: ")(body).flush();
}

BufferRequest::BufferRequest(const Request& request, const std::string& header, const std::string& body) :
	Request(const_cast<IClientSocket&>(request.getSocket()), request.incomingIP, request.incomingPort)
{
	mBuffer = header;

	size_t pos = mBuffer.find("\r\n");
    std::string line = mBuffer.substr(0, pos);
    mBuffer.erase(0, pos + 2);
    parseRequestLine(line);
	parse();
	mBuffer.clear();
	mBody = body;
	mReader = new DefaultReader(mSocket, body.length());
}

BufferRequest::BufferRequest( const BufferRequest& b ): Request(b)
{}

BufferRequest::~BufferRequest()
{}

BufferRequest&	BufferRequest::operator=( const BufferRequest& b )
{
	(void)b;
	return (*this);
}

int	BufferRequest::getSocketFd() const { return -1; }

std::string	BufferRequest::read()
{
	std::string	ret = mBody;
	mBody.clear();
	Logger::debug ("buffer request ret : ")(ret).flush();
	return (ret);
}

bool	BufferRequest::done() const {
    return (mBody.empty());
}

void	BufferRequest::dump(bool colors) const
{
	Request::dump(colors);
	Logger::debug ("Buffer:\n")(mBuffer).flush();
}
