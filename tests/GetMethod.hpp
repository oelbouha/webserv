#ifndef GETMETHOD_HPP
#define GETMETHOD_HPP

#include <iostream>


#include "src/Response/Response.hpp"
#include "src/Response/ProxiedResponse.hpp"
#include "src/Request/Request.hpp"
#include "src/DataTypes/Config.hpp"



class GetMethod
{
	public:
		GetMethod();
		GetMethod( const GetMethod& s );
		GetMethod&	operator=( const GetMethod& s );
		~GetMethod();

		IResponse*	handle(const IRequest& Request);

	private:
		unsigned int statusCode;
		
};
#endif