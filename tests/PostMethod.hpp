#ifndef POSTMETHOD_HPP
#define POSTMETHOD_HPP

#include <iostream>

#include "src/Response/Response.hpp"
#include "src/Response/ProxiedResponse.hpp"
#include "src/Request/Request.hpp"
#include "src/DataTypes/Config.hpp"

class PostMethod
{
	public:
		PostMethod();
		PostMethod( const PostMethod& s );
		PostMethod&	operator=( const PostMethod& s );
		~PostMethod();

		IResponse*	handle(const IRequest& Request);

	private:
		unsigned int statusCode;
		
};


#endif