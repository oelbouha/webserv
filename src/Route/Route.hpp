/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Route.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysalmi <ysalmi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/19 18:46:33 by oelbouha          #+#    #+#             */
/*   Updated: 2024/02/05 17:54:01 by ysalmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ROUTE_HPP
#define ROUTE_HPP

#include <iostream>

#include "src/Server/Result.hpp"

#include "src/Response/Response.hpp"
#include "src/Request/Request.hpp"
#include "src/DataTypes/Config.hpp"
#include "src/Server/Server.hpp"
#include "src/Server/ErrorPage.hpp"
#include "src/Route/Upload.hpp"
#include "src/Interfaces/IHandler.hpp"
#include "Utils.hpp"

class Upload;

class Route : public IHandler {
	std::vector<string> 	allowedMethods;
	std::vector<string> 	CGIExtensions;
	std::vector<Config*> 	redirect;
	ErrorPage&				error_pages;
	Config&					config;
	string					autoindex;
	string					URI;
	string					path;
	string					root;
	string					indexfile;
	string  				uploadPath;
	unsigned int			statusCode;

public:
	Route(Config * config, ErrorPage& pages);
	Route&	operator=( const Route& s );
	~Route();

	Config& getConfig() const;
	std::vector<string>	getAllowedMethods() const;
	const string&	getURI() const;
	const string&	getRoot() const;

	unsigned int getStatusCode() const ;
	
	const ErrorPage& 		getErrorPage() const;
	const string	setMethod(const std::string& method);
	string			BuildDirectoryListingHtmlPage();
	
	bool		hasRedirection() const;
	bool		IsResourceFileExist(const string& uri) const;
	bool		hasCGIExtension(const string& uri) const;
	bool		IsMethodAllowed(const std::string& method);
	bool 		canDeleteFile(const string& filePath) const;
	bool 		canReadFile(const string& filePath) const ;

	int 		DeleteFile();
	std::vector<string> ReadDirectory();
	
	IResponse*	handleDirectory(const IRequest&);
	IResponse*	deleteDirectory(const IRequest&);
	IResponse*	handleRequestedFile(const IRequest&);
	IResponse*	ProcessRequestMethod(IRequest& );
	IResponse*	ExecuteGETMethod(const IRequest&);
	IResponse*	ExecutePOSTMethod(IRequest&);
	IResponse*	ExecuteDELETEMethod(const IRequest&);
	IResponse*	ExecuteHEADMethod(const IRequest&);
	
	Result  handle(IRequest&);

private:
	
};

#endif
