/*                       __          _
 *     __  ___________ _/ /___ ___  (_)
 *    / / / / ___/ __ `/ / __ `__ \/ /
 *   / /_/ (__  ) /_/ / / / / / / / / 
 *   \__, /____/\__,_/_/_/ /_/ /_/_/ 
 *  /____/  User: Youssef Salmi
 *          File: ISocket.hpp 
 */

#pragma once
#ifndef ISOCKT_HPP
#define ISOCKT_HPP

#include <string>

class ISocket
{

public:
    virtual ~ISocket(){};

    virtual int         getID() const = 0;

    virtual void        bind(unsigned int aIP, unsigned int aPORT) = 0;
    virtual void        listen() = 0;
    virtual ISocket*    accept() = 0;
    virtual void        close() = 0;

    virtual void    setNonBlocking() = 0;

    // write buffer to socket and return how many bytes were written
    virtual int         write(const char* buffer, int size) = 0;

    // one read call - returns empty string if no data is found
    virtual std::string read(unsigned int aMaxSize) = 0;

    // read all availabe data on socket
    virtual std::string readAll() = 0;

    virtual void        dump() = 0;

};


#endif