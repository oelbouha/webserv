/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Route.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysalmi <ysalmi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/19 18:46:33 by oelbouha          #+#    #+#             */
/*   Updated: 2024/02/07 13:08:00 by ysalmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ROUTE_HPP
#define ROUTE_HPP

#include <iostream>

#include <sys/errno.h>

#include "src/Server/Result.hpp"

#include "src/Response/Response.hpp"
#include "src/Request/Request.hpp"
#include "src/DataTypes/Config.hpp"
#include "src/Server/ErrorPage.hpp"
#include "src/Route/Upload.hpp"
#include "src/CGI/CGIHandler.hpp"
#include "Utils.hpp"

class Upload;

class Route {
	std::vector<string> 	allowedMethods;
	std::vector<string> 	CGIExtensions;
	std::vector<Config*> 	redirect;
	ErrorPage&				error_pages;
	bool					autoindex;
	string					uri;
	string					location;
	string					root;
	string					indexfile;
	string  				uploadPath;
	unsigned int			code;

public:
	Route(Config * config, ErrorPage& pages);
	Route&	operator=( const Route& s );
	~Route();
	
	const string&		getURI() const;
	std::string 		getAbsolutePath(std::string requri);
	const ErrorPage& 	getErrorPage() const;
	bool				IsMethodAllowed(const std::string& method);

	Result  			handle(IRequest&);

private:
	IResponse*  			makeFileResponseFromPath(const IRequest& request, const std::string& path);
	IResponse*				handleRequestToFile(const IRequest&);
	Result					handleRequestToCgi(IRequest&);
	IResponse*				makeDirectoryListingResponse(const IRequest& request, const std::string& path);
	std::vector<string> 	ReadDirectory(const std::string& path);
	bool					isRequestToCgi(const std::string & );
};

#endif
