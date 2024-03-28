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

Request::Request(IClientSocket &aSocket, int aIncomingIP, int aIncomingPort):
    mSocket(aSocket),
    mReader(NULL),
    incomingIP(aIncomingIP),
    incomingPort(aIncomingPort),
    error(false),
    mMaxBodySize(0)
{}

Request::Request(const Request &r) :
    mSocket(r.mSocket),
    mReader(NULL),
    incomingIP(r.incomingIP),
    incomingPort(r.incomingPort),
    error(false),
    mMethod(r.mMethod),
    mUri(r.mUri),
    mQuery(r.mQuery),
    mHttpVersion(r.mHttpVersion),
    mHeaders(r.mHeaders),
    mMaxBodySize(r.mMaxBodySize)
{}

Request::~Request() 
{
    delete mReader;
}

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

void    Request::setHeader(const std::string& key, const std::string& val)
{
    if (!key.empty())
        mHeaders[key] = val;
}

const IClientSocket &Request::getSocket() const { return mSocket; }

int Request::getSocketFd() const { return mSocket.getSocketFd(); }

const std::string&  Request::getMethod() const { return (mMethod); }

const std::string&  Request::getURI() const { return (mUri); }

const std::string&  Request::getHttpVersion() const { return (mHttpVersion); }

const std::string&  Request::getQuery() const { return (mQuery); }

const std::string&  Request::getHeader(const std::string &aKey) const
{
    string_string_map::const_iterator val = mHeaders.find(aKey);
    if (val == mHeaders.end())
        return mNoHeader;
    return val->second;
}

void            Request::setMaxBodySize(uint size)
{
    mMaxBodySize = size;
}

void Request::readHeader()
{
    mBuffer = mSocket.readHeaderOnly();
    if (mBuffer.empty())
        throw RequestException("Incomplete Header");

    size_t pos = mBuffer.find("\r\n");
    std::string line = mBuffer.substr(0, pos);
    mBuffer.erase(0, pos + 2);
    parseRequestLine(line);
}

void Request::build()
{
    if (mBuffer.empty()) return;
    try {
        std::istringstream ss(mBuffer);
        std::string line;

        mBuffer.clear();
        while (std::getline(ss, line))
            parseHeaderProperty(line);

        string_string_map::iterator te = mHeaders.find("transfer-encoding");
        if (te != mHeaders.end() && te->second == "chunked")
            mReader = new ChunkedReader(mSocket);
        else {
            string_string_map::iterator cl = mHeaders.find("content-length");
            unsigned int content_length = 0;
            if (cl != mHeaders.end())
                content_length = utils::string_to_uint(cl->second);
            mReader = new DefaultReader(mSocket, content_length);
        }
    }
    catch (const std::exception& )
    { throw RequestException(RequestException::BAD_REQUEST); }
}

std::string Request::read()
{
    std::string ret = mReader->read();
    if (mMaxBodySize && mReader->getContentLength() > mMaxBodySize)
        throw RequestException(RequestException::CLIENT_BODY_TOO_LARGE);
    return (ret);
}

bool      Request::done() const
{
    return (mReader->eof());
}

void Request::parse()
{
    std::istringstream ss(mBuffer);
    std::string line;

    while (std::getline(ss, line))
        parseHeaderProperty(line);
}

void Request::parseRequestLine(const std::string &aRequestLine) {
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

    mUri = utils::decode_url(word.substr(0, uriEnd));
    mQuery = word.substr(queryStart);

    ss >> word;
    mHttpVersion = word.substr(word.find("/") + 1);
}

void Request::parseHeaderProperty(const std::string &aHeaderLine)
{
    std::string key, value;

    size_t collonPos = aHeaderLine.find(":");

    if (collonPos == std::string::npos)
        throw RequestException(aHeaderLine + " : Unkown header format");

    key = aHeaderLine.substr(0, collonPos);
    value = aHeaderLine.substr(collonPos + 1);

    utils::trim_spaces(key);
    utils::trim_spaces(value);

    if (key.empty())
        throw RequestException(aHeaderLine + "Unkown header format");
    utils::str_to_lower(key);

    mHeaders[key] = value;
}

std::string Request::getRawHeader() const
{
    std::string raw_header = mMethod + " " + mUri + " " + mQuery + " HTTP/1.1\r\n";

    string_string_map::const_iterator it = mHeaders.begin();
    for (; it != mHeaders.end(); ++it)
        raw_header += it->first + ": " + it->second + "\r\n";
    return (raw_header);
}

void Request::dump(bool colors) const
{
    if (colors)
        Logger::debug ("\e[32m");

    Logger::debug (mMethod)(" ");
    Logger::debug (mUri)(" ");
    Logger::debug (mQuery)(" ");
    Logger::debug ("HTTP/")(mHttpVersion)("\e[0m").flush();

    string_string_map::const_iterator it = mHeaders.begin();
    for (; it != mHeaders.end(); ++it)
        Logger::debug.l().w(20, it->first)(": ")(it->second).flush();
}
