
#ifndef IHANDLER_HPP
#define IHANDLER_HPP

#include "src/Response/Response.hpp"
#include "src/Response/ProxiedResponse.hpp"
#include "src/Request/Request.hpp"
#include "src/DataTypes/Config.hpp"
#include "Utils.hpp"

class IHandler
{
    public:
        virtual	~IHandler(){};
        virtual IResponse*  handle( const IRequest& req ) = 0;
};

#endif
