/*	                     __          _
 *	   __  ___________ _/ /___ ___  (_)
 *	  / / / / ___/ __ `/ / __ `__ \/ /
 *	 / /_/ (__  ) /_/ / / / / / / / /
 *	 \__, /____/\__,_/_/_/ /_/ /_/_/
 *	/____/	User: Youssef Salmi
 *			File: ProxiedResponse.cpp
 */

#include "ProxiedResponse.hpp"

ProxiedResponse::ProxiedResponse(IClientSocket &sock, int inputFd, int outputFd)
: mSocket(sock),
    mInput(inputFd),
    mOutput(outputFd),
    mIsForwardingComplete(false),
    mIsSendingComplete(false),
    mIsHeaderComplete(false) 
{
    setNonBlocking();
}

ProxiedResponse::ProxiedResponse(const ProxiedResponse &p)
: mSocket(p.mSocket),
    mInput(p.mInput),
    mOutput(p.mOutput),
    mIsForwardingComplete(p.mIsForwardingComplete),
    mIsSendingComplete(p.mIsSendingComplete),
    mIsHeaderComplete(p.mIsHeaderComplete) 
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

void    ProxiedResponse::setIsHeaderComplete(bool val)
{
    mIsHeaderComplete = val;
}

bool    ProxiedResponse::isHeaderComplete() { return mIsHeaderComplete; }

void    ProxiedResponse::completeHeader() {
   mOutputBuffer = readHeader(); 

   // parse header
   
   // add needed details

   // convert to string -> mOutputBuffer

}

void    ProxiedResponse::forward()
{
    // read all that can be read
    mInputBuffer += mSocket.readAll();
    // send all tha can be sent
    writeAll(mInput, mInputBuffer);
}

void    ProxiedResponse::send()
{
    // read all that can be read
    readAll(mOutput, mOutputBuffer);
    // write all
    int written = mSocket.writeAll(mOutputBuffer);
    mOutputBuffer.erase(0, written);
}

/*
 *
 *
-----------------------------9051914041544843365972754266
Content-Disposition: form-data; name="file1"; filename="a.txt"
Content-Type: text/plain

Content of a.txt.

-----------------------------9051914041544843365972754266
Content-Disposition: form-data; name="file1"; filename="a.txt"
Content-Type: text/plain
File-Path: /tmpfle1


 */




void ProxiedResponse::writeAll(int fd, std::string& buffer)
{
    int r;
    while (buffer.length())
    {
        r = ::write(fd, buffer.data(), buffer.length());
        if (r < 0)
            break;
        buffer.erase(0, r);
    }
}

void ProxiedResponse::readAll(int fd, std::string& buffer)
{
    size_t  size = 200000;
    char buff[size];
    int r;
    while (true)
    {
        r = ::read(fd, buff, size);
        if (r < 0)
            break;
        buffer += std::string(buffer, r);
    }
}

void    ProxiedResponse::setNonBlocking()
{
  if (fcntl(mInput, F_SETFL, O_NONBLOCK, O_CLOEXEC) < 0)
    throw std::invalid_argument("Can't switch socket to non-blocking mode");
  if (fcntl(mOutput, F_SETFL, O_NONBLOCK, O_CLOEXEC) < 0)
    throw std::invalid_argument("Can't switch socket to non-blocking mode");
}
