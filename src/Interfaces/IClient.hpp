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

#include "IClientSocket.hpp"
#include "IRequest.hpp"
#include "IResponse.hpp"

class IClient {
    public:
        virtual ~IClient(){};

        virtual void                    makeRequest() = 0;
        virtual bool                    hasRequest() const = 0;
        virtual IRequest*               getRequest() = 0;
        virtual int                     getSocketFd() const = 0;
        virtual const IClientSocket&    getSocket() const = 0;
};
#endif
