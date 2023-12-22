/*	                     __          _
 *	   __  ___________ _/ /___ ___  (_)
 *	  / / / / ___/ __ `/ / __ `__ \/ /
 *	 / /_/ (__  ) /_/ / / / / / / / / 
 *	 \__, /____/\__,_/_/_/ /_/ /_/_/ 
 *	/____/	User: Youssef Salmi
 *			File: IProxiedResponse.hpp 
 */

#pragma once
#ifndef IPROXIEDRESPONSE_HPP
#define IPROXIEDRESPONSE_HPP

class IProxiedResponse
{
public:
	virtual		~IProxiedResponse(){};
    virtual void        setCGIInput( int fildes ) = 0;
    virtual void        setCGIOutput( int fildes ) = 0;

    virtual void        setIsHeaderComplete() = 0;
    virtual bool        isHeaderComplete() = 0;
    virtual void        completeHeader() = 0;

    virtual void        forward() = 0;
    virtual void        send() = 0;

    virtual bool        isSendingComplete() const = 0;
    virtual bool        isFrowardingComplete() const = 0;
};
#endif
