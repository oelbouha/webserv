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

class ISocket
{

    public:
        virtual ~ISocket(){};
        virtual void    bind() = 0;
        virtual void    connect() = 0;
        virtual void    listen() = 0;
        virtual ISocket accept() = 0;
}


#endif