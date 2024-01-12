#include "PostMethod.hpp"

PostMethod::PostMethod() {
	statusCode = 200;
}

PostMethod::PostMethod( const PostMethod& s ) {(void)s;}

PostMethod::~PostMethod() {}

PostMethod&	PostMethod::operator=( const PostMethod& s )
{
    (void)s;
	return (*this);
}

IResponse*  PostMethod::handle(const IRequest& request)
{
	(void)request;
	Response *res = NULL;

	return res;
}