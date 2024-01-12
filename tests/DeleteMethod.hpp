#ifndef DELETEMETHOD_HPP
#define DELETEMETHOD_HPP

#include <iostream>

#include "src/Response/Response.hpp"
#include "src/Response/ProxiedResponse.hpp"
#include "src/Request/Request.hpp"
#include "src/DataTypes/Config.hpp"

class DeleteMethod
{
	public:
		DeleteMethod();
		DeleteMethod( const DeleteMethod& s );
		DeleteMethod&	operator=( const DeleteMethod& s );
		~DeleteMethod();

		IResponse*	handle(const IRequest& Request);

	private:
		unsigned int statusCode;
		
};


#endif