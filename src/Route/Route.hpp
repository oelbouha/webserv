

#pragma once
#ifndef ROUTE_HPP
#define ROUTE_HPP

#include <iostream>

#include "src/Response/Response.hpp"
#include "src/Response/ProxiedResponse.hpp"
#include "src/Request/Request.hpp"
#include "src/DataTypes/Config.hpp"
#include "src/Methods/Method.hpp"
#include "src/Server/Server.hpp"
#include "Utils.hpp"

class Server;

class Route{
	std::vector<string> allowedMethods;
	std::map<string, string> resHeader;
	unsigned int		statusCode;
	bool				hasRedirect;
	
public:
	Route(const Config * config);
	Route( const Route& s );
	Route&	operator=( const Route& s );
	~Route();

	const Config*		getConfig() const;
	std::vector<string>	getAllowedMethods() const;
	std::string	getURI() const;

	const string		setMethod(method_t m);
	bool		hasRedirection() const;
	bool		IsResourceFileExist(const string& uri) const;
	bool		hasCGIExtension(const string& uri) const;
	bool		IsMethodAllowed(method_t method);

	
	IResponse*	handleDirectory(const IRequest&);
	IResponse*	deleteDirectory(const IRequest&);
	IResponse*	handleRequestedFile(const IRequest&);
	IResponse*  handle(const IRequest&);
	IResponse*	ProcessRequestMethod(const IRequest& );
	IResponse*	ExecuteGETMethod(const IRequest&);
	IResponse*	ExecutePOSTMethod(const IRequest&);
	IResponse*	ExecuteDELETEMethod(const IRequest&);
	IResponse*	ExecuteHEADMethod(const IRequest&);

	const string	getMimeType(const string& uri);
private:
	string	URI;
	string	root;
	string	indexfile;
};

#endif
