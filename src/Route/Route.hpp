/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Route.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelbouha <oelbouha@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/19 18:46:33 by oelbouha          #+#    #+#             */
/*   Updated: 2024/01/26 15:54:32 by oelbouha         ###   ########.fr       */
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
	const string	setMethod(method_t m);
	string			BuildDirectoryListingHtmlPage();
	
	bool		hasRedirection() const;
	bool		IsResourceFileExist(const string& uri) const;
	bool		hasCGIExtension(const string& uri) const;
	bool		IsMethodAllowed(method_t method);
	bool 		canDeleteFile(const string& filePath) const;
	bool 		canReadFile(const string& filePath) const ;

	int 		DeleteFile();
	std::vector<string> ReadDirectory();
	
	IResponse*	handleDirectory(const IRequest&);
	IResponse*	deleteDirectory(const IRequest&);
	IResponse*	handleRequestedFile(const IRequest&);
	IResponse*  handle(const IRequest&);
	IResponse*	ProcessRequestMethod(const IRequest& );
	IResponse*	ExecuteGETMethod(const IRequest&);
	IResponse*	ExecutePOSTMethod(const IRequest&);
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
