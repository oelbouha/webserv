

#include "DeleteMethod.hpp"

DeleteMethod::DeleteMethod() {
	statusCode = 200;
}

DeleteMethod::DeleteMethod( const DeleteMethod& s ) {(void)s;}

DeleteMethod::~DeleteMethod() {}

DeleteMethod&	DeleteMethod::operator=( const DeleteMethod& s )
{
    (void)s;
	return (*this);
}

IResponse*  DeleteMethod::handle(const IRequest& request)
{
	(void)request;
	Response *res = NULL;

	return res;
}