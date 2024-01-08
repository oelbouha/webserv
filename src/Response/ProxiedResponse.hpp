/*	                     __          _
 *	   __  ___________ _/ /___ ___  (_)
 *	  / / / / ___/ __ `/ / __ `__ \/ /
 *	 / /_/ (__  ) /_/ / / / / / / / / 
 *	 \__, /____/\__,_/_/_/ /_/ /_/_/ 
 *	/____/	User: Youssef Salmi
 *			File: ProxiedResponse.hpp 
 */

#pragma once
#ifndef PROXIEDRESPONSE_HPP
#define PROXIEDRESPONSE_HPP

#include <string>

#include <unistd.h>
#include <fcntl.h>

#include "IProxiedResponse.hpp"

#include "src/Interfaces/IClientSocket.hpp"

class ProxiedResponse : public IProxiedResponse
{
  IClientSocket&    mSocket;
  int               mInput;
  int               mOutput;

  bool              mIsForwardingComplete;
  bool              mIsSendingComplete;

  bool              mIsHeaderComplete;

  std::string       mInputBuffer;
  std::string       mOutputBuffer;

public:
	ProxiedResponse(IClientSocket& sock, int inputFd, int outputFd);
	ProxiedResponse( const ProxiedResponse& p);
	~ProxiedResponse();

	ProxiedResponse&	operator=( const ProxiedResponse& p );
    
    virtual void        setIsHeaderComplete(bool isHeaderComplete);
    virtual bool        isHeaderComplete();
    virtual void        completeHeader();

    virtual void        forward();
    virtual void        send();

    virtual bool        isSendingComplete() const;
    virtual bool        isFrowardingComplete() const;

private:
    std::string         readHeader();
    void                writeAll(int fd, std::string& buffer);
    void                readAll(int fd, std::string& buffer);
    void                setNonBlocking();
};
#endif
