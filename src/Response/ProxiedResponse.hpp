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

#include "src/Interfaces/IRequest.hpp"
#include "src/Interfaces/IClientSocket.hpp"
#include "src/Response/ProxiedResponse.hpp"


class ProxiedResponse : public IProxiedResponse
{
  IRequest&         mRequest;
  const IClientSocket&    mSocket;

  int               mInput;
  int               mOutput;

  bool              mIsForwardingComplete;
  bool              mdone;


  bool              mIsHeaderComplete;

  std::string       mInputBuffer;
  std::string       mOutputBuffer;

  int               mForwarded;
  bool              mOutputEOF;

public:
	ProxiedResponse(IRequest& req, int inputFd, int outputFd);

	~ProxiedResponse();

	ProxiedResponse&	operator=( const ProxiedResponse& p );
    
    virtual void        setIsHeaderComplete(bool isHeaderComplete);
    virtual bool        isHeaderComplete();
    virtual void        completeHeader();

    virtual void        forward();
    virtual void        send();

    virtual bool        isSendingComplete() const;
    virtual bool        isFrowardingComplete() const;

  virtual int         getInputFd() const;
  virtual int         getOutputFd() const;
  virtual int         getSocketFd() const;

  virtual IResponse&  setStatusCode( unsigned int aStatusCode );
  virtual IResponse&  setHeader( const std::string& aKey, const std::string& aValue );
  virtual IResponse&  build();

  virtual bool        done() const;

private:
    std::string         readHeader();
    int                 writeAll(int fd, std::string& buffer);
    int                 readAll(int fd, std::string& buffer);
    void                setNonBlocking();

    virtual IResponse&  setBody( const std::string& aBody );
    virtual IResponse&  setBodyFile( const std::string& aFileName );
};
#endif
