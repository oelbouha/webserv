/*	                     __          _
 *	   __  ___________ _/ /___ ___  (_)
 *	  / / / / ___/ __ `/ / __ `__ \/ /
 *	 / /_/ (__  ) /_/ / / / / / / / / 
 *	 \__, /____/\__,_/_/_/ /_/ /_/_/ 
 *	/____/	User: Youssef Salmi
 *			File: IMultiplexer.hpp 
 */

#pragma once
#ifndef IMULTIPLEXER_HPP
#define IMULTIPLEXER_HPP

#include <queue>

#include "ISocket.hpp"

enum class SocketMonitor
{
    rdonly,
    wronly,
    rw
};

class IMultiplexer
{
public:
    virtual ~IMultiplexer(){};

    /*
            Adds/Removes a socket to be monitored for when it's ready for the
        requested operation.
            If the Socket already beeing monitored just add the monitoring mode
        specified by @aMode.
    */
    virtual void        addSocket( const ISocket& aSocket, SocketMonitor aMode ) = 0;
    virtual void        removeSocket( const ISocket& aSocket ) = 0;

    /*
            Should block until a socket or are ready for the requested operation
        or until @timeout milisecond elapses.
    */
    virtual void        watch( uint64_t timeout ) = 0;

    /*
            Check if there are any ready sockets for operations since the last watch
        call.
    */
    virtual bool        hasReadySockets() = 0;

    /*
            After a watch call return, the first time this method is invoked it returns
        a pointer to the first ready socket and each subsequent call to this method
        it should return the next ready socket.
    */
    virtual ISocket*    getNextReadySocket() = 0;

};
#endif