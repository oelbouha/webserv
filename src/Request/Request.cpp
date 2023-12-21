/*	                     __          _
 *	   __  ___________ _/ /___ ___  (_)
 *	  / / / / ___/ __ `/ / __ `__ \/ /
 *	 / /_/ (__  ) /_/ / / / / / / / /
 *	 \__, /____/\__,_/_/_/ /_/ /_/_/
 *	/____/	User: Youssef Salmi
 *			File: Request.cpp
 */

#include "Request.hpp"
#include <string>

Request::Request(IClientSocket &aSocket, int aIncomingIP, int aIncomingPort)
    : mSocket(aSocket), mIncommingIP(aIncomingIP),
      mIncommingPort(aIncomingPort) {}

Request::Request(const Request &r)
    : mSocket(r.mSocket), mMethod(r.mMethod), mUri(r.mUri), mQuery(r.mQuery),
      mHttpVersion(r.mHttpVersion), mHeaders(r.mHeaders),
      mIncommingIP(r.mIncommingIP), mIncommingPort(r.mIncommingPort) {}

Request::~Request() {}

Request &Request::operator=(const Request &r) {
  if (this != &r) {
    mMethod = r.mMethod;
    mUri = r.mUri;
    mQuery = r.mQuery;
    mHttpVersion = r.mHttpVersion;
    mHeaders = r.mHeaders;
  }
  return (*this);
}

IClientSocket &Request::getSocket() const { return mSocket; }

int Request::getIncomingIP() const { return mIncommingIP; }

int Request::getIncomingPort() const { return mIncommingPort; }

method_t Request::getMethod() const { return (mMethod); }

const std::string &Request::getURI() const { return (mUri); }

const std::string &Request::getHttpVersion() const { return (mHttpVersion); }

const std::string &Request::getQuery() const { return (mQuery); }

const std::string &Request::getHeader(const std::string &aKey) const {
  try {
    return (mHeaders.at(aKey));
  } catch (...) {
    throw RequestException("No such header");
  }
}

void Request::build() { parse(); }

void Request::parse() {
  std::string header = mSocket.readHeaderOnly();
  if (header.empty())
      throw RequestException(RequestException::CONNECTION_COLOSED);
  
  std::istringstream ss(header);
  std::string line;

  std::getline(ss, line);
  parseRequestLine(line);

  while (std::getline(ss, line))
    parseHeaderProperty(line);
}

void Request::parseRequestLine(const std::string &aRequestLine) {
  std::istringstream ss(aRequestLine);
  std::string word;

  ss >> word;
  setMethod(word);

  ss >> word;
  size_t uriEnd = word.find("?");
  size_t queryStart = uriEnd + 1;

  if (uriEnd == std::string::npos) {
    uriEnd = word.length();
    queryStart = uriEnd;
  }

  mUri = word.substr(0, uriEnd);
  mQuery = word.substr(queryStart);

  ss >> word;
  mHttpVersion = word.substr(word.find("/") + 1);
}

void Request::parseHeaderProperty(const std::string &aHeaderLine) {
  std::string key, value;

  size_t collonPos = aHeaderLine.find(":");

  if (collonPos == std::string::npos)
    throw RequestException(aHeaderLine + "Unkown header format");

  key = aHeaderLine.substr(0, collonPos);
  value = aHeaderLine.substr(collonPos + 1);

  utils::trimSpaces(key);
  utils::trimSpaces(value);

  if (key.empty() || value.empty())
    throw RequestException(aHeaderLine + "Unkown header format");

  mHeaders[key] = value;
}

void Request::setMethod(const std::string &aMethod) {
  if (aMethod == "GET")
    mMethod = GET;
  else if (aMethod == "POST")
    mMethod = POST;
  else if (aMethod == "HEAD")
    mMethod = HEAD;
  else if (aMethod == "DELETE")
    mMethod = DELETE;
  else
    throw RequestException("Unkown http mMethod");
}

void Request::dump(bool colors) const {
  using std::cout;
  using std::endl;
  using std::flush;

  cout << "Method: ";
  if (colors)
    cout << "\e[32m";
  switch (mMethod) {
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
  cout << mUri << "\e[0m\n";

  cout << "Query: ";
  if (colors)
    cout << "\e[32m";
  cout << mQuery << "\e[0m\n";

  cout << "Http Version: ";
  if (colors)
    cout << "\e[32m";
  cout << mHttpVersion.substr(mHttpVersion.find('/') + 1) << "\e[0m\n";

  std::map<std::string, std::string>::const_iterator it = mHeaders.begin();
  for (; it != mHeaders.end(); ++it)
    std::cout << std::left << std::setw(20) << it->first << ": " << it->second
              << std::endl;

  std::string body = mSocket.readAll();

  cout << body << endl;

  cout << flush;
}
