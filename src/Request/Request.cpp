/*	                     __          _
 *	   __  ___________ _/ /___ ___  (_)
 *	  / / / / ___/ __ `/ / __ `__ \/ /
 *	 / /_/ (__  ) /_/ / / / / / / / / 
 *	 \__, /____/\__,_/_/_/ /_/ /_/_/ 
 *	/____/	User: Youssef Salmi
 *			File: Request.cpp 
 */

#include "Request.hpp"

Request::Request( IClientSocket& aSocket ) :
mSocket(aSocket)
{}

Request::Request( const Request& r ):
method(r.method),
uri(r.uri),
query(r.query),
httpVersion(r.httpVersion),
headers(r.headers),
mSocket(r.mSocket)
{}

Request::~Request()
{}

Request&	Request::operator=( const Request& r )
{
	if (this != &r)
	{
		method = r.method;
		uri = r.uri;
		query = r.query;
		httpVersion = r.httpVersion;
		headers = r.headers;
	}
	return (*this);
}

/*
// method_t	Request::getMethod() const
// {
// 	return (mMethod);
// }

// const std::string&	Request::getURI() const
// {
// 	return (mURI);
// }

// const std::string&	Request::getHttpVersion() const
// {
// 	return (mHttpVersion);
// }

// const std::string&	Request::getQuery() const
// {
// 	return (mQuery);
// }
*/

const std::string&	Request::getHeader( const std::string& aKey) const
{
	try{
		return (headers.at(aKey));
	} catch (...)
	{
		throw RequestException("No such header");
	}
}

void	Request::build()
{
	parse();
}

void	Request::dump(bool colors) const
{
	using std::cout;
	using std::endl;
	using std::flush;

	cout << "Method: ";
	if (colors)
		cout << "\e[32m";
	switch (method)
	{
		case GET:
			cout << "GET";
			break;
		case POST:
			cout << "POST";
			break;
		default:
			break;
	}

	cout << "\e[0m" << endl;

	cout << "URI: ";
	if (colors)
		cout << "\e[32m";
	cout << uri << "\e[0m\n";
	
	cout << "Params: ";
	if (colors)
		cout << "\e[32m";
	cout << query << "\e[0m\n";

	cout << "Http Version: ";
	if (colors)
		cout << "\e[32m";
	cout << httpVersion.substr(httpVersion.find('/') + 1) << "\e[0m\n";

	std::map<std::string, std::string>::const_iterator	it = headers.begin();
	for (;it != headers.end();++it)
		std::cout << std::left << std::setw(20) << it->first << ": " << it->second << std::endl;

	cout << flush;
}



void	Request::parse()
{
	// std::string			rawHeader = mSocket.readHeaderOnly();
	std::istringstream	ss(mSocket.readHeaderOnly());

	std::string		line;

	std::getline(ss, line);

	parseRequestLine(line);

	while (std::getline(ss, line))
		parseHeaderProperty(line);
}

void	Request::parseRequestLine( const std::string& aRequestLine )
{
	std::istringstream	ss(aRequestLine);
	std::string			word;

	ss >> word;
	setMethod(word);

	ss >> word;
	size_t	uriEnd = word.find("?");
	size_t	queryStart = uriEnd + 1;

	if (uriEnd == std::string::npos)
	{
		uriEnd = word.length();
		queryStart = uriEnd;
	}

	uri = word.substr(0, uriEnd);
	query = word.substr(queryStart);

	ss >> word;
	httpVersion = word.substr(word.find("/") + 1);
}

void	Request::parseHeaderProperty( const std::string& aHeaderLine )
{
	std::string	key, value;

	size_t	collonPos = aHeaderLine.find(":");

	if (collonPos == std::string::npos )
		throw RequestException(aHeaderLine + "Unkown header format");

	key = aHeaderLine.substr(0, collonPos);
	value = aHeaderLine.substr(collonPos + 1);

	utils::trimSpaces(key);
	utils::trimSpaces(value);

	if (key.empty() || value.empty())
		throw RequestException(aHeaderLine + "Unkown header format");

	headers[key] = value;
}

void	Request::setMethod( const std::string& aMethod )
{
	if (aMethod == "GET")
		method = GET;
	else if (aMethod == "POST")
		method = POST;
	else if (aMethod == "HEAD")
		method = HEAD;
	else if (aMethod == "DELETE")
		method = DELETE;
	else
		throw RequestException("Unkown http method");
}