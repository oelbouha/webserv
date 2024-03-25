/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Route.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysalmi <ysalmi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/19 18:46:25 by oelbouha          #+#    #+#             */
/*   Updated: 2024/03/23 18:13:24 by ysalmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Route.hpp"

Route::Route(Config* config, ErrorPages& pages): error_pages(pages)
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
			AResponse::StatusCodes.at(code);
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

const std::string&	Route::getURI() const{ return uri; }

bool				Route::isMethodAllowed(const std::string& method)
{
	for(size_t i = 0; i < allowedMethods.size(); ++i) {
		if (method == allowedMethods[i])
			return true;
	}
	return false;
}

std::vector<DirLisingItem>	Route::readDirectory(const std::string& path)
{
	std::vector<DirLisingItem> list;

	DIR* dir = opendir(path.c_str());
	if (!dir) throw std::invalid_argument("Permession Denied");
	
	struct dirent* entry;
	DirLisingItem	tmp;
	while ((entry = readdir(dir)) != NULL) {
		if (entry->d_name[0] == '.')
			continue;
		tmp.name = entry->d_name;
		tmp.isDir = (entry->d_type == DT_DIR);
		list.push_back(tmp);
	}
	closedir(dir);
	std::sort(list.begin(), list.end());
	return list;
}

IResponse*			Route::makeDirectoryListingResponse(const Request& request, const std::string& path)
{	
	std::vector<DirLisingItem> fileList = readDirectory(path);
	
	std::string body = DIR_LISTING_START;
	for (size_t i = 0; i < fileList.size(); ++i)
	{
		DirLisingItem& item = fileList[i];
		std::string line;
		
		if (item.isDir) line = DIR_ITEM;
		else line = FILE_ITEM;
		
		utils::replace(line, "::path::", utils::encode_url(item.name));
		utils::replace(line, "::name::", item.name);
		body += line;
	}
	body += DIR_LISTING_END;

	IResponse * response = new BufferResponse(request.getSocket());
	response->setStatusCode(200).setBody(body)
		.setHeader("content-type", "text/html");
	return response;
}

IResponse*  		Route::makeFileResponseFromPath(const Request& request, const std::string& path) {
	try
	{
		IResponse * response = new FileResponse(request.getSocket());
		response->setStatusCode(200).setBody(path);
		return response;
	}
	catch(const ResponseException& e)
	{
		int status_code = e.error == ResponseException::PERMISSION_DENIED ? 403 : 404;
		return (error_pages.build(request, status_code));
	}
}

IResponse*  		Route::handleRequestToFile(const Request& request)
{
	std::string path = getAbsolutePath(request.getURI());

	if (utils::is_directory(path))
	{
		if (path.back() != '/')
		{
			const string& uri = request.getURI();
			IResponse * response = new BufferResponse(request.getSocket());	
			response->setHeader("Location", "http://" + request.getHeader("host") + uri + "/")
				.setHeader("Content-Length", "0")
				.setStatusCode(301)
				.build();
			return (response);
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
				// return (error_pages.build(request, 403));
				// log error
			}
		}
		return (error_pages.build(request, 403));
	}
	return makeFileResponseFromPath(request, path);
}

Result  			Route::handleRequestToCgi(Request& request)
{
	std::string path = getAbsolutePath(request.getURI());
	
	if ( ::access(path.c_str(), F_OK) == -1)
		return (Result(error_pages.build(request, 404)));
	
	if ( ::access(path.c_str(), R_OK | X_OK) == -1 )
		return (Result(error_pages.build(request, 500)));
	
	if (
		request.getMethod() == "POST" &&
		!uploadPath.empty() &&
		request.getHeader("content-type").find("multipart/form-data") != std::string::npos &&
        request.getHeader("x-upload") != "false"
	)
		return (Result(new Upload(&request, uploadPath)));

	ProxyPair	pair = CGIHandler::handle(&request, path);

	if (pair.request == NULL) (Result(error_pages.build(request, 500)));

	CGIRequest* req = static_cast<CGIRequest*>(pair.request);
	req->setErrorPages(&error_pages);
	CGIResponse* res = static_cast<CGIResponse*>(pair.response);
	res->setErrorPages(&error_pages);
	
	return Result(pair);
}

std::string 		Route::getAbsolutePath(std::string req_uri)
{
	req_uri = utils::decode_url(req_uri);
	req_uri.erase(0, uri.length());
	if (!req_uri.empty() && req_uri.front() != '/')
		req_uri = "/" + req_uri;
	return (root + req_uri);
}

bool				Route::isRequestToCgi(const std::string & aUri)
{
	if (CGIExtensions.empty()) return false;

	std::string extension = '.' + utils::getExtension(aUri);

	if (std::find(CGIExtensions.begin(), CGIExtensions.end(), extension) != CGIExtensions.end())
		return true;
	return false;
}

Result  			Route::handle(Request& request)
{
	// request.setMaxBodySize(100);
	
	if ( ! isMethodAllowed(request.getMethod()) )
		return (Result(error_pages.build(request, 405)));
	
	if ( ! redirect.empty() )
	{
		BufferResponse* response = new BufferResponse(request.getSocket());
		response->setStatusCode(code).setBody("")
			.setHeader("location", location);
		return Result(response);
	}
	
	if ( isRequestToCgi(request.getURI()) )
		return handleRequestToCgi(request);
		
	if ( request.getMethod() == "GET" ) {
		IResponse *res = handleRequestToFile(request);
		return (Result(res));		
	}
	
	return (Result(error_pages.build(request, 501)));
}
