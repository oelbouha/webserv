/*                       __          _
 *     __  ___________ _/ /___ ___  (_)
 *    / / / / ___/ __ `/ / __ `__ \/ /
 *   / /_/ (__  ) /_/ / / / / / / / / 
 *   \__, /____/\__,_/_/_/ /_/ /_/_/ 
 *  /____/  User: Youssef Salmi
 *          File: IClient.hpp 
 */

#pragma once
#ifndef ICLIENT_HPP
#define ICLIENT_HPP

#include "IRequest.hpp"

class IClient
{
    public:
        virtual ~IClient(){};
        virtual void        activate() = 0;
        virtual void        fillRequest() = 0;
        virtual bool        hasRequest() = 0;
        virtual IRequest*   getRequest() = 0;

};
#endif