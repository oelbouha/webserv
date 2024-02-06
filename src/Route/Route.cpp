/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Route.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelbouha <oelbouha@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/19 18:46:25 by oelbouha          #+#    #+#             */
/*   Updated: 2024/02/06 19:00:05 by oelbouha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Route.hpp"

Route::Route(Config* config, ErrorPage& pages): error_pages(pages)
{
	redirect = config->getBlockConfigIfExist("redirect");

	if (redirect.size() > 1)
		throw ConfigException("Webserv", "redirect", "Multiple Declaration");
	if (redirect.size())
	{
		location = redirect.front()->getInlineConfigIfExist("location");
		if (location.empty())
			throw ConfigException("Webserv", "redirect", "redirect Block Without Location");
		
		std::string prop = redirect.front()->getInlineConfigOr("code", "301");
		try {
			code = utils::string_to_uint(prop);
			if (code < 300 || code >= 400)
				throw std::invalid_argument("Invalid code");
			Response::sStatusCodes.at(code);
		}
		catch(const std::exception& e) {
			throw ConfigException("Not a Valid Redirect Code Number", "code", prop);
		}
	}

	autoindex = config->getInlineConfigOr("autoindex", "no") == "yes";
	
	uri = config->getInlineConfigIfExist("uri");
	if (uri.back() == '/')
		uri.erase(uri.length() - 1);

	root = config->getInlineConfigIfExist("root");
	uploadPath = config->getInlineConfigIfExist("upload");
	indexfile = config->getInlineConfigIfExist("index");
	allowedMethods = config->getListConfigIfExist("allowed_methods");
	CGIExtensions =  config->getListConfigIfExist("cgi");
}

Route&	Route::operator=( const Route& s ) {
    (void)s;
	return (*this);
}

Route::~Route() {}

const ErrorPage& 	Route::getErrorPage() const { return error_pages; }

const std::string&	Route::getURI() const{ return uri; }

bool	Route::IsMethodAllowed(const std::string& method) {
	for(size_t i = 0; i < allowedMethods.size(); ++i){
		if (method == allowedMethods[i])
			return true;
	}
	return false;
}


std::vector<string> Route::ReadDirectory(const std::string& path)
{
	std::vector<string> list;

	DIR* dir = opendir(path.c_str());
	if (!dir)
		throw std::invalid_argument("Permession Denied");
	struct dirent* entry;
	while ((entry = readdir(dir)) != NULL) 
		if (entry->d_name[0] != '.')
			list.push_back(entry->d_name);
	closedir(dir);
	return list;
}

IResponse*	Route::makeDirectoryListingResponse(const IRequest& request, const std::string& path)
{	
	std::vector<string> fileList = ReadDirectory(path);
	
	std::string body = "<h1> Directory Listing </h1> <ul>";
	for (size_t i = 0; i < fileList.size(); ++i)
	{
		string line = "<li> <a href=\"" + fileList[i] + "\">" + fileList[i] + "</a> </li> ";
		body += line;
	}
	body += "</ul>";

	IResponse * response = new Response(request.getSocket());
	response->setHeader("content-type", "text/html")
		.setStatusCode(200)
		.setBody(body)
		.build();
	return response;
}


IResponse*  Route::makeFileResponseFromPath(const IRequest& request, const std::string& path) {
	try
	{
		IResponse * response = new Response(request.getSocket());
		response->setHeader("connection", request.getHeader("Connection"))
			.setStatusCode(200)
			.setBodyFile(path)
			.build();
		return response;
	}
	catch(...)
	{
		int status_code = errno == EACCES ? 403 : 404;
		return (error_pages.build(request, status_code));
	}
}

IResponse*  Route::handleRequestToFile(const IRequest& request) {
	
	std::string path = getAbsolutePath(request.getURI());

	if (utils::IsDirectory(path))
	{
		if (path.back() != '/')
		{
			const string& uri = request.getURI();
			IResponse * response = new Response(request.getSocket());	
			response->setHeader("Location", "http://" + request.getHeader("host") + uri + "/")
				.setHeader("Content-Length", "0")
				.setStatusCode(301)
				.build();
			return response;
		}
		else if (!indexfile.empty())
			return (makeFileResponseFromPath(request, path + indexfile));
		else if (autoindex)
		{
			try
			{
				IResponse *response = makeDirectoryListingResponse(request, path);
				return response;
			}
			catch(...){
				return makeFileResponseFromPath(request, path);
			}
		}
		return (error_pages.build(request, 403));
	}
	return makeFileResponseFromPath(request, path);
}

Result  Route::handleRequestToCgi(IRequest& request)
{
	if (request.getMethod() == "POST" && \
		!uploadPath.empty() && \
		request.getHeader("content-type").find("multipart/form-data") != std::string::npos
	)
	{
		Upload *upload = new Upload(&request, uploadPath);
		
		// std::string buff = upload->handle(request);
		// while (true) {
		// 	if (upload->IsDone())
		// 		break ;
		// 	buff += upload->handle(request);
		// }
		
		// statusCode = 201;
		std::cout << "Upload......\n" << std::flush;
		return (Result(upload));
	}
	return (Result(error_pages.build(request, 400)));
}

std::string Route::getAbsolutePath(std::string requri) {
	requri.erase(0, uri.length());
	if (!requri.empty() && requri.front() != '/')
		requri = "/" + requri;
	return (root + requri);
}

Result  Route::handle(IRequest& request)
{	
	if (IsMethodAllowed(request.getMethod()) == false)
		return (Result(error_pages.build(request, 405)));
	else if (!redirect.empty())
	{
		IResponse * response = new Response(request.getSocket());
		response->setHeader("location", location)
			.setStatusCode(code)
			.build();
		return Result(response);
	}
	if (isRequestToCgi(request.getURI()))
	{
		return handleRequestToCgi(request);
		// IResponse * response = new Response(request.getSocket());
		// response->setBody("CGI")
		// 	.setStatusCode(200)
		// 	.build();
		// return Result(response);
	}
	if (request.getMethod() == "GET") {
		IResponse *res = handleRequestToFile(request);
		return (Result(res));		
	}
	return (Result(error_pages.build(request, 415)));
}

bool	Route::isRequestToCgi(const std::string & aUri) {
	if (!CGIExtensions.empty())
	{
		std::string extension = utils::getExtension(aUri);
		extension.insert(extension.begin(), '.');
		if (std::find(CGIExtensions.begin(), CGIExtensions.end(), extension) != CGIExtensions.end())
			return true;
	}
	return false;
}