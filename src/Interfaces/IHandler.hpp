
#ifndef IHANDLER_HPP
#define IHANDLER_HPP

#include "src/Response/ProxiedResponse.hpp"
#include "src/Request/Request.hpp"
#include "src/Server/ErrorPage.hpp"
#include "Utils.hpp"

class IResponse;
class ErrorPage;

class IHandler
{
    public:
        virtual	~IHandler(){};
        // virtual IResponse*          handle(IRequest& req ) = 0;
        virtual const ErrorPage& 	getErrorPage() const = 0;
        virtual const string&       getRoot() const = 0;
        virtual unsigned int        getStatusCode() const = 0;
};

#endif
