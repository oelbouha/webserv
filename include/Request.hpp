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
#include <map>

/*
GET /?key=value HTTP/2
Host: c.6sc.co
User-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10.15; rv:109.0) Gecko/20100101 Firefox/114.0
Accept-Language: en-GB,en;q=0.5
Accept-Encoding: gzip, deflate, br
Origin: https://rapidapi.com
Connection: keep-alive
Referer: https://rapidapi.com/
Cookie: 6suuid=c2681cb878872e002c3b5365b8020000d54f0200
Sec-Fetch-Dest: empty
Sec-Fetch-Mode: cors
Sec-Fetch-Site: cross-site
Method: my method
*/

class Request
{
	// first;
	std::string	mMethod;
	std::string	mUri;
	std::string	mHttpVersion;
	std::string	mQuery;
	std::string	mHost;
	std::map<std::string, std::string>	mHeaders;
	std::string	mBody;

	// second:
	std::map<std::string, std::string>	mData;

	public:
		Request( void );
		Request( Request& r );
		~Request();

		Request&	operator=( Request& r );

};
#endif
