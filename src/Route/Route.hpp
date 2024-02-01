/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Route.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysalmi <ysalmi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/19 18:46:33 by oelbouha          #+#    #+#             */
/*   Updated: 2024/02/01 09:28:29 by ysalmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ROUTE_HPP
#define ROUTE_HPP

#include <iostream>

#include "src/Response/Response.hpp"
#include "src/Response/ProxiedResponse.hpp"
#include "src/Request/Request.hpp"
#include "src/DataTypes/Config.hpp"
#include "src/Methods/Method.hpp"
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

public:
	Route(Config * config, ErrorPage& pages);
	Route&	operator=( const Route& s );
	~Route();

	Config& getConfig() const;
	std::vector<string>	getAllowedMethods() const;
	string	getURI() const;
	string	getRoot() const;

	unsigned int getStatusCode() const ;
	
	ErrorPage& 		getErrorPage() const;
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
	IResponse*  handle(IRequest&);
	IResponse*	ProcessRequestMethod(IRequest& );
	IResponse*	ExecuteGETMethod(const IRequest&);
	IResponse*	ExecutePOSTMethod(IRequest&);
	IResponse*	ExecuteDELETEMethod(const IRequest&);
	IResponse*	ExecuteHEADMethod(const IRequest&);

private:
	ErrorPage		&error_pages;
	Config			&config;
	string			autoindex;
	string			URI;
	string			path;
	string			root;
	string			indexfile;
	string  		uploadPath;
	unsigned int	statusCode;
};

#endif
