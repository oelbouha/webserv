

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
	bool				hasRedirection;
	
public:
	Route(const Config * config);
	Route( const Route& s );
	Route&	operator=( const Route& s );
	~Route();

	const Config*		getConfig() const;
	std::vector<string>	getAllowedMethods() const;
	std::string	getURI() const;

	void		setMethod(method_t m);
	Route&		setResponseHeader(const string& key, const string& value);
	Route&		setStatusCode(unsigned int);
	bool		hasRedirect() const;
	bool		IsResourceFileExist(string& uri) const;
	bool		hasCGIExtension(string& uri) const;
	bool		IsMethodAllowed() const;

	
	IResponse*	handleDirectory(const IRequest& request);
	IResponse*	deleteDirectory(const IRequest& request);
	IResponse*	handleFile(const IRequest& request);
	IResponse*  handle(const IRequest&);
	IResponse*	WhichMethodTouse(const IRequest& );
	IResponse*	HandleGET(const IRequest& Request);
	IResponse*	HandlePOST(const IRequest& Request);
	IResponse*	HandleDELETE(const IRequest& Request);
	IResponse*	HandleHEAD(const IRequest& Request);

	const string	getMimeType(const string& uri);
private:
	string	method;
	string	URI;
	string	root;
	string	indexfile;
};

#endif
