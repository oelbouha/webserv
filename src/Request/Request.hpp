/*	                     __          _
 *	   __  ___________ _/ /___ ___  (_)
 *	  / / / / ___/ __ `/ / __ `__ \/ /
 *	 / /_/ (__  ) /_/ / / / / / / / / 
 *	 \__, /____/\__,_/_/_/ /_/ /_/_/ 
 *	/____/	User: Youssef Salmi
 *			File: Request.hpp 
 */

#pragma once
#ifndef REQUEST_HPP
#define REQUEST_HPP

#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <map>

#include "IRequest.hpp"
#include "IClientSocket.hpp"
#include "RequestException.hpp"
#include "Utils.hpp"

struct Request
{
	method_t		method;
	std::string		uri;
	std::string		query;
	std::string		httpVersion;
	std::map<std::string, std::string>	headers;

	Request( IClientSocket& mSocket );
	Request( const Request& aRequest );
	~Request();

	Request&	operator=( const Request& aRequest );

    const std::string&  getHeader( const std::string& aKey ) const;

	void	build();

	void	dump(bool colors = true) const;

	// method_t            getMethod();
    // const std::string&  getURI();
    // const std::string&  getHttpVersion();
    // const std::string&  getQuery();
private:
	IClientSocket&	mSocket;

	void	parse();
	void	parseRequestLine( const std::string& aRequestLine );
	void	parseHeaderProperty( const std::string& aHeaderLine);
	void	setMethod( const std::string& aMethod );
};
#endif
