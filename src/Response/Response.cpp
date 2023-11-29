/*	                     __          _
 *	   __  ___________ _/ /___ ___  (_)
 *	  / / / / ___/ __ `/ / __ `__ \/ /
 *	 / /_/ (__  ) /_/ / / / / / / / / 
 *	 \__, /____/\__,_/_/_/ /_/ /_/_/ 
 *	/____/	User: Youssef Salmi
 *			File: Response.cpp 
 */

#include "Response.hpp"


Response::Response( IClientSocket& aSocket ) :
mSocket(aSocket),
mCursor(0)
{}

Response::Response( const Response& aResponse ) :
mSocket(aResponse.mSocket),
mCursor(0)
{}

Response::~Response()
{}

Response&	Response::operator=( const Response& aResponse )
{
	(void)aResponse;
	return (*this);
}

Response&	Response::setStatusCode( unsigned int aStatusCode )
{
	mStatusCode = aStatusCode;
	return (*this);
}

Response&	Response::setHeader( const std::string& aHeader, const std::string& aValue )
{
	mHeaders[aHeader] = aValue;
	return (*this);
}

Response&	Response::setBody( const std::string& aBody )
{
	mBody = aBody;
	mHeaders["content-length"] = std::to_string( mBody.length() );
	return (*this);
}

Response&	Response::build()
{
	mRawResponse = "HTTP/1.1 " + Response::sStatusCodes.at(mStatusCode) + "\r\n";
	for (string2string::iterator it = mHeaders.begin(); it != mHeaders.end(); ++it)
		mRawResponse += it->first + ": " + it->second + "\r\n";
	mRawResponse += "\r\n" + mBody;
	return (*this);
}

Response&	Response::startSending()
{
	return (*this);
}

void		Response::send()
{
	const char*	buff = mRawResponse.c_str() + mCursor;

	// std::cout << "mCursor: " << mCursor << std::endl << std::flush;
	size_t	size = mRawResponse.length() - mCursor;
	// size = size>65536?65536:size;

	mCursor += mSocket.write(buff, size);
}

bool		Response::isSendingComplete()
{
	return (mCursor == mRawResponse.length());
}

void		Response::dump()
{
	std::cout << mRawResponse << std::endl << std::flush;
}

void		Response::dumpHeader()
{
	std::cout << mRawResponse.substr(0, mRawResponse.find("\r\n\r\n")) << std::endl << std::flush;
}

const std::map<unsigned int, std::string>	Response::sStatusCodes = Response::initStatusCodes();

std::map<unsigned int, std::string> Response::initStatusCodes()
{
	std::map<unsigned int, std::string>	statusMap;

	{// status codes 1xx
		statusMap[100] = "100 Continue";
		statusMap[101] = "101 Switching protocols";
		statusMap[102] = "102 Processing";
		statusMap[103] = "103 Early Hints";
	}
	
	{// status codes 2xx
		statusMap[200] = "200 OK";
		statusMap[201] = "201 Created";
		statusMap[202] = "202 Accepted";
		statusMap[203] = "203 Non-Authoritative Information";
		statusMap[204] = "204 No Content";
		statusMap[205] = "205 Reset Content";
		statusMap[206] = "206 Partial Content";
		statusMap[207] = "207 Multi-Status";
		statusMap[208] = "208 Already Reported";
		statusMap[226] = "226 IM Used";
	}
	
	{// status codes 3xx
		statusMap[300] = "300 Multiple Choices";
		statusMap[301] = "301 Moved Permanently";
		statusMap[302] = "302 Found (Previously \"Moved Temporarily\")";
		statusMap[303] = "303 See Other";
		statusMap[304] = "304 Not Modified";
		statusMap[305] = "305 Use Proxy";
		statusMap[306] = "306 Switch Proxy";
		statusMap[307] = "307 Temporary Redirect";
		statusMap[308] = "308 Permanent Redirect";
	}

	{// status codes 4xx
		statusMap[400] = "400 Bad Request";
		statusMap[401] = "401 Unauthorized";
		statusMap[402] = "402 Payment Required";
		statusMap[403] = "403 Forbidden";
		statusMap[404] = "404 Not Found";
		statusMap[405] = "405 Method Not Allowed";
		statusMap[406] = "406 Not Acceptable";
		statusMap[407] = "407 Proxy Authentication Required";
		statusMap[408] = "408 Request Timeout";
		statusMap[409] = "409 Conflict";
		statusMap[410] = "410 Gone";
		statusMap[411] = "411 Length Required";
		statusMap[412] = "412 Precondition Failed";
		statusMap[413] = "413 Payload Too Large";
		statusMap[414] = "414 URI Too Long";
		statusMap[415] = "415 Unsupported Media Type";
		statusMap[416] = "416 Range Not Satisfiable";
		statusMap[417] = "417 Expectation Failed";
		statusMap[418] = "418 I'm a Teapot";
		statusMap[421] = "421 Misdirected Request";
		statusMap[422] = "422 Unprocessable Entity";
		statusMap[423] = "423 Locked";
		statusMap[424] = "424 Failed Dependency";
		statusMap[425] = "425 Too Early";
		statusMap[426] = "426 Upgrade Required";
		statusMap[428] = "428 Precondition Required";
		statusMap[429] = "429 Too Many Requests";
		statusMap[431] = "431 Request Header Fields Too Large";
		statusMap[451] = "451 Unavailable For Legal Reasons";
	}
	
	{// status codes 5xx
		statusMap[500] = "500 Internal Server Error";
		statusMap[501] = "501 Not Implemented";
		statusMap[502] = "502 Bad Gateway";
		statusMap[503] = "503 Service Unavailable";
		statusMap[504] = "504 Gateway Timeout";
		statusMap[505] = "505 HTTP Version Not Supported";
		statusMap[506] = "506 Variant Also Negotiates";
		statusMap[507] = "507 Insufficient Storage";
		statusMap[508] = "508 Loop Detected";
		statusMap[510] = "510 Not Extended";
		statusMap[511] = "511 Network Authentication Required";
	}
	
	return statusMap;
}