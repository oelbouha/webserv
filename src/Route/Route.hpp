

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

class Route
{
	public:
		Route(const Config * config);
		Route( const Route& s );
		Route&	operator=( const Route& s );
		~Route();

		std::vector<std::string>	getAllowedMethods() const;
		std::string	getURI() const;

		const Config*		getConfig() const;
		void		setMethod(method_t m);
		bool		hasRedirect() const;
		bool		IsExist(string& uri) const;
		bool		isCGI(string& uri) const;
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

	private:
		unsigned int		statusCode;
		std::vector<string> allowedMethods;
		bool				hasRedirection;
		string				method;
		string				URI;
		string				root;
		string				indexfile;
};

#endif
