/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Route.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysalmi <ysalmi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/19 18:46:25 by oelbouha          #+#    #+#             */
/*   Updated: 2024/03/28 22:49:20 by ysalmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Route.hpp"

Route::Route(Config* config, ErrorPages& pages) :
	error_pages(pages),
	max_body_size(0)
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
	
	try {
		max_body_size = utils::string_to_uint(
			config->getInlineConfigOr("max_body_size", "0")
		);
	} catch (const std::exception&) {
		throw ConfigException(
			"Not a Valid Number",
			"max_body_size",
			config->getInlineConfig("max_body_size"));
	}
	
	
	autoindex = config->getInlineConfigOr("autoindex", "no") == "yes";
	
	uri = config->getInlineConfigIfExist("uri");
	if (uri[uri.length() - 1] == '/') uri.erase(uri.length() - 1);

	root = config->getInlineConfigIfExist("root");
	if (root[root.length() - 1] == '/') root.pop_back();
	
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

Result  			Route::handle(Request& request)
{
	request.setMaxBodySize(max_body_size);

	if (! isMethodImplemented(request.getMethod()))
		return (Result(error_pages.build(request, 501)));
	
	if ( ! isMethodAllowed(request.getMethod()) ) {
		IResponse* res = error_pages.build(request, 405);
		res->setHeader("Allow", getAllowedMethods());
		return Result(res);
	}
	
	if ( ! redirect.empty() )
	{
		BufferResponse* response = new BufferResponse(request.getSocket());
		response->setStatusCode(code).setBody("")
			.setHeader("Location", location);
		return Result(response);
	}
	
	if ( isRequestToCgi(request.getURI()) )
		return handleRequestToCgi(request);
		
	if ( request.getMethod() == "GET" ) {
		IResponse *res = handleRequestToFile(request);
		return (Result(res));		
	}
	IResponse* res = error_pages.build(request, 405);
	res->setHeader("Allow", "GET");
	return Result(res);
}

bool				Route::isMethodImplemented(const std::string& method)
{
	if (method == "GET") return true;
	if (method == "POST") return true;
	if (method == "DELETE") return true;
	return false;
}

std::string 		Route::getAbsolutePath(std::string req_uri)
{
	std::string uri_tmp = req_uri;
	req_uri.erase(0, uri.length());
	
	std::string path = root + req_uri;
	
	if (uri_tmp[uri_tmp.length() - 1] == '/') uri_tmp.pop_back();

	bool trailing_slash = path[path.length() - 1] == '/';

	if (utils::is_directory(path) && uri_tmp == uri && trailing_slash && !indexfile.empty())
		path += indexfile;
	return (path);
}

std::string			Route::getAllowedMethods() const
{
	std::string	allowed_methods;
	std::vector<std::string>::const_iterator it = allowedMethods.begin();
	while (true) {
		allowed_methods += *it;
		if (++it == allowedMethods.end())
			break;
		allowed_methods += ", ";
	}
	return allowed_methods;
}

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
	std::string 				body = DIR_LISTING_START;
	
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
		.setHeader("Content-Type", "text/html");
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
		if (path[path.length() - 1] != '/')
		{
			const string& uri = request.getURI();
			const string& host = request.getHeader("host");
			const string& query = request.getQuery();
			string location = "http://" + host + uri + "/";
			if (!query.empty()) location += "?" + query;
			IResponse * response = new BufferResponse(request.getSocket());	
			response->setStatusCode(301)
				.setHeader("Content-Length", "0")
				.setHeader("Location", location);
			return (response);
		}
		else if (autoindex)
		{
			try { return makeDirectoryListingResponse(request, path); }
			catch(...){
				return (error_pages.build(request, 403));
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
	
	if (isUpload(request)) {
		if ( ::access(uploadPath.c_str(), F_OK | W_OK) == -1)
			return (Result(error_pages.build(request, 403)));
		return (Result(new Upload(&request, uploadPath)));
	}

	ProxyPair	pair = CGIHandler::handle(&request, path);

	if (pair.request == NULL) (Result(error_pages.build(request, 500)));

	CGIRequest* req = static_cast<CGIRequest*>(pair.request);
	req->setErrorPages(&error_pages);
	CGIResponse* res = static_cast<CGIResponse*>(pair.response);
	res->setErrorPages(&error_pages);
	
	return Result(pair);
}

bool	Route::isUpload(const Request& request)
{
	bool is_upload = request.getMethod() == "POST";
	is_upload = is_upload && !uploadPath.empty();

	const std::string& ct = request.getHeader("content-type");
	is_upload = is_upload && ct.find("multipart/form-data") != std::string::npos;
    is_upload = is_upload && request.getHeader("x-upload") != "false";
	return is_upload;
}

bool				Route::isRequestToCgi(const std::string & aUri)
{
	if (CGIExtensions.empty()) return false;
	
	std::string extension = '.' + utils::getExtension(getAbsolutePath(aUri));
	if (std::find(CGIExtensions.begin(), CGIExtensions.end(), extension) != CGIExtensions.end())
		return true;
	return false;
}
