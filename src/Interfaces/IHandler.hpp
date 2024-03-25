
#ifndef IHANDLER_HPP
#define IHANDLER_HPP

#include "src/Request/Request.hpp"
#include "src/Server/ErrorPages.hpp"
#include "Utils.hpp"

class IResponse;
class ErrorPages;

class IHandler
{
    public:
        virtual	~IHandler(){};
        // virtual IResponse*          handle(IRequest& req ) = 0;
        virtual const ErrorPages& 	getErrorPages() const = 0;
        virtual const string&       getRoot() const = 0;
        virtual unsigned int        getStatusCode() const = 0;
};

#endif
