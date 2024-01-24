/*	                     __          _
 *	   __  ___________ _/ /___ ___  (_)
 *	  / / / / ___/ __ `/ / __ `__ \/ /
 *	 / /_/ (__  ) /_/ / / / / / / / /
 *	 \__, /____/\__,_/_/_/ /_/ /_/_/
 *	/____/	User: Youssef Salmi
 *			File: Request.cpp
 */

#include "Request.hpp"
#include "RequestException.hpp"
#include <string>
#include <cctype>

/* need to somewhere else*/
std::string&  str_to_lower(std::string& str)
{
    for (std::string::iterator it = str.begin(); it != str.end(); ++it)
        *it = std::tolower(*it);
    return (str);
}

Request::Request(IClientSocket &aSocket, int aIncomingIP, int aIncomingPort):
    mSocket(aSocket),
    mReader(NULL),
    mIncommingIP(aIncomingIP),
    mIncommingPort(aIncomingPort)
{}

Request::Request(const Request &r) :
    mSocket(r.mSocket)
{}

Request::~Request() {}

Request &Request::operator=(const Request &r)
{
    if (this != &r) {
        mMethod = r.mMethod;
        mUri = r.mUri;
        mQuery = r.mQuery;
        mHttpVersion = r.mHttpVersion;
        mHeaders = r.mHeaders;
    }
    return (*this);
}

const IClientSocket &Request::getSocket() const { return mSocket; }

int Request::getIncomingIP() const { return mIncommingIP; }

int Request::getIncomingPort() const { return mIncommingPort; }

const std::string&  Request::getMethod() const { return (mMethod); }

const std::string&  Request::getURI() const { return (mUri); }

const std::string&  Request::getHttpVersion() const { return (mHttpVersion); }

const std::string&  Request::getQuery() const { return (mQuery); }

const std::string&  Request::getHeader(const std::string &aKey) const
{
    try
    {
        return (mHeaders.at(aKey));
    }
    catch (...)
    {
        throw RequestException("No such header");
    }
}

size_t  Request::getContentLength() const 
{ 
    return (mReader->getContentLength());
}

void Request::build()
{
    try
    {
        parse();
        try
        {
            if (mHeaders.at("transfer-encoding") != "chunked")
                throw false;
            mReader = new ChunkedRequestReader(mSocket);
        }
        catch (...)
        {
            std::string cl;
            try
            {
                cl = mHeaders.at("content-length");
            } catch(...)
            {
                cl = "0";
            }

            size_t  contentLength = std::stod(cl.data(), NULL);

            std::cout << "content length(str): " << cl << std::endl;
            std::cout << "content length(nbr): " << contentLength 
                << std::endl << std::flush;

            mReader = new DefaultRequestReader(mSocket, contentLength);
        }
    }
    catch (const SocketException& e)
    {
        (void)e;
        throw RequestException(RequestException::CONNECTION_COLOSED);
    }
}

std::string Request::read()
{
    return (mReader->read());
}

bool      Request::done() const
{
    return (mReader->eof());
}

void Request::parse()
{
    std::string header = mSocket.readHeaderOnly();

    if (header.empty())
        return ;

    std::istringstream ss(header);
    std::string line;

    std::getline(ss, line);
    parseRequestLine(line);

    while (std::getline(ss, line))
        parseHeaderProperty(line);
}

void Request::parseRequestLine(const std::string &aRequestLine)
{
    std::istringstream ss(aRequestLine);
    std::string word;

    ss >> mMethod;

    ss >> word;
    size_t uriEnd = word.find("?");
    size_t queryStart = uriEnd + 1;

    if (uriEnd == std::string::npos)
    {
        uriEnd = word.length();
        queryStart = uriEnd;
    }

    mUri = word.substr(0, uriEnd);
    mQuery = word.substr(queryStart);

    ss >> word;
    mHttpVersion = word.substr(word.find("/") + 1);
}

void Request::parseHeaderProperty(const std::string &aHeaderLine)
{
    std::string key, value;

    size_t collonPos = aHeaderLine.find(":");

    if (collonPos == std::string::npos)
        throw RequestException(aHeaderLine + "Unkown header format");

    key = aHeaderLine.substr(0, collonPos);
    value = aHeaderLine.substr(collonPos + 1);

    utils::trimSpaces(key);
    utils::trimSpaces(value);

    str_to_lower(key);
    str_to_lower(value);

    std::cout << "|" << key << "|" << std::endl;

    if (key.empty())
        throw RequestException(aHeaderLine + "Unkown header format");

    mHeaders[key] = value;
}

void Request::dump(bool colors) const
{
    using std::cout;
    using std::endl;
    using std::flush;

    {
        if (colors)
            cout << "\e[32m";

        cout << mMethod << " ";
        cout << mUri << " ";
        cout << mQuery << " ";
        cout << "HTTP/" << mHttpVersion << "\e[0m\n";

        std::map<std::string, std::string>::const_iterator it = mHeaders.begin();
        for (; it != mHeaders.end(); ++it)
            std::cout << std::left << std::setw(20) << it->first << ": " << it->second
                << std::endl;
    }

    cout << "----- body -----\n" << flush;
    std::string body = mReader->read();

    cout << body << endl << flush;
}
