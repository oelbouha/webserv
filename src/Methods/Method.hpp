#ifndef METHOD_HPP
#define METHOD_HPP

#include <iostream>

#include "src/Response/Response.hpp"
#include "src/Response/ProxiedResponse.hpp"
#include "src/Request/Request.hpp"
#include "src/DataTypes/Config.hpp"


class Method
{
	public:
		Method();
		Method( const Method& s );
		Method&	operator=( const Method& s );
		~Method();

		IResponse*	GET(const IRequest& Request);
		IResponse*	POST(const IRequest& Request);
		IResponse*	DELETE(const IRequest& Request);

	private:
		unsigned int statusCode;
		
};


#endif