/*	                     __          _
 *	   __  ___________ _/ /___ ___  (_)
 *	  / / / / ___/ __ `/ / __ `__ \/ /
 *	 / /_/ (__  ) /_/ / / / / / / / /
 *	 \__, /____/\__,_/_/_/ /_/ /_/_/
 *	/____/	User: Youssef Salmi
 *			File: ProxiedResponse.cpp
 */

#include "ProxiedResponse.hpp"
#include <cstdlib>

ProxiedResponse::ProxiedResponse(IRequest& req, int inputFd, int outputFd) :
    mRequest(req),
    mSocket(req.getSocket()),
    mInput(inputFd),
    mOutput(outputFd),
    mIsForwardingComplete(false),
    mIsSendingComplete(false),
    mIsHeaderComplete(false),
    mForwarded(0),
    mOutputEOF(false)
{
    setNonBlocking();
}

ProxiedResponse::~ProxiedResponse() {
    close(mInput);
    close(mOutput);
}

ProxiedResponse &ProxiedResponse::operator=(const ProxiedResponse &p) {
  (void)p;
  return (*this);
}

int         ProxiedResponse::getInputFd() const
{
    return (mInput);
}

int         ProxiedResponse::getOutputFd() const
{
    return (mOutput);
}

int         ProxiedResponse::getSocketFd() const
{
    return (mSocket.getSocketFd());
}

IResponse&  ProxiedResponse::setBodyFile(const std::string& file)
{
    (void)file;
    return *this;
}

IResponse&  ProxiedResponse::setBody(const std::string& body)
{
    (void)body;
    return *this;
}

IResponse&  ProxiedResponse::setStatusCode(unsigned int statusCode)
{
    (void)statusCode;
    return *this;
}

IResponse&  ProxiedResponse::build()
{
    return *this;
}

IResponse&  ProxiedResponse::setHeader(const std::string& key, const std::string& value)
{
    (void)key;
    (void)value;
    return *this;
}


void    ProxiedResponse::setIsHeaderComplete(bool val)
{
    mIsHeaderComplete = val;
}

bool    ProxiedResponse::isHeaderComplete() { return mIsHeaderComplete; }

void    ProxiedResponse::completeHeader()
{
   //mOutputBuffer = readHeader(); 

   // parse header
   
   // add needed details

   // convert to string -> mOutputBuffer
}

void    ProxiedResponse::forward()
{
    if (mRequest.getMethod() == "GET")
        return;
    // read all that can be read
    mInputBuffer += mRequest.read();
    // send all tha can be sent
    mForwarded += writeAll(mInput, mInputBuffer);
}

void    ProxiedResponse::send()
{
    // read all that can be read
    if (readAll(mOutput, mOutputBuffer) == 0)
    {
        //if (sent == 0){
        //    mOutputBuffer = "";
        //    sendError = true;
        //}
        //else
            mOutputEOF = true;
    }
    // write all
    int written = mSocket.writeAll(mOutputBuffer);

    //sent += written;
    
    mOutputBuffer.erase(0, written);

   // if (sendError && mOutputBuffer.empty())
   //     mOutputEOF = true;
}


bool    ProxiedResponse::isFrowardingComplete() const
{
    if (mRequest.getMethod() == "GET")
        return (true);

    int content_length = std::atoi(mRequest.getHeader("content-length").data());

    return (mForwarded == content_length);
}

bool ProxiedResponse::isSendingComplete() const
{
    return (mOutputEOF && mOutputBuffer.empty());
}

/*
-----------------------------9051914041544843365972754266
Content-Disposition: form-data; name="file1"; filename="a.txt"
Content-Type: text/plain

Content of a.txt.

-----------------------------9051914041544843365972754266
Content-Disposition: form-data; name="file1"; filename="a.txt"
Content-Type: text/plain
File-Path: /tmpfle1
*/




int ProxiedResponse::writeAll(int fd, std::string& buffer)
{
    int ret = 0;
    int r;
    while (buffer.length())
    {
        r = ::write(fd, buffer.data(), buffer.length());
        if (r <= 0)
            break;
        ret += r;
        buffer.erase(0, r);
    }
    return (ret);
}

int ProxiedResponse::readAll(int fd, std::string& buffer)
{
    int ret = buffer.length();
    size_t  size = 200000;
    char    buff[size];
    int     r;

    while (true)
    {
        r = ::read(fd, buff, size);
        if (r < 0)
            break;
        ret += r;
        buffer += std::string(buffer, r);
    }
    return (ret);
}

void    ProxiedResponse::setNonBlocking()
{
  if (fcntl(mInput, F_SETFL, O_NONBLOCK, O_CLOEXEC) < 0)
    throw std::invalid_argument("Can't switch socket to non-blocking mode");
  if (fcntl(mOutput, F_SETFL, O_NONBLOCK, O_CLOEXEC) < 0)
    throw std::invalid_argument("Can't switch socket to non-blocking mode");
}

