/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Route.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelbouha <oelbouha@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/19 18:46:25 by oelbouha          #+#    #+#             */
/*   Updated: 2024/01/22 23:50:24 by oelbouha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Route.hpp"

Route::Route(Config* config, ErrorPages& pages): error_pages(pages)
{
	autoindex =  config->getInlineConfigIfExist("autoindex");
	location =  config->getInlineConfigIfExist("location");
	URI =  config->getInlineConfigIfExist("uri");
	root =  config->getInlineConfigIfExist("root");
	uploadPath =  config->getInlineConfigIfExist("upload");
	indexfile =  config->getInlineConfigIfExist("index");
	allowedMethods = config->getListConfigIfExist("allowed_methods");
	error_pages.setErrorPage(config, root);
}

Route&	Route::operator=( const Route& s ){
    (void)s;
	return (*this);
}

Route::~Route() {}

std::vector<std::string>	Route::getAllowedMethods() const{ return allowedMethods; }

std::string	Route::getURI() const{ return URI; }

std::string	Route::getRoot() const{ return root; }

bool	Route::hasRedirection() const {
	return location.size();
}

bool Route::canDeleteFile(const string& filePath) const {
    return (access(filePath.c_str(), W_OK) == 0);
}

bool Route::canReadFile(const string& filePath) const {
    return (access(filePath.c_str(), R_OK) == 0);
}

bool	Route::IsResourceFileExist(const string& filePath) const{
	return access(filePath.c_str(), F_OK) == 0;
}

bool	Route::hasCGIExtension(const string& uri) const{
	
	if (CGIExtensions.size() == 0)
		return false;
	std::string extension = getExtension(uri);
	std::vector<string>::const_iterator it = CGIExtensions.begin();
	while (it != CGIExtensions.end()){
		if (*it == extension)
			return true;
		++it;
	}
	return false;
}

bool	Route::IsMethodAllowed(method_t m){
	const string & method = setMethod(m);
	for(size_t i = 0; i < allowedMethods.size(); ++i){
		if (method == allowedMethods[i])
			return true;
	}
	return false;
}

const string	Route::setMethod(method_t m){
	switch (m){
	case GET:
		return "GET";
	case POST:
		return "POST";
	case DELETE:
		return "DELETE";
	case HEAD:
		return "HEAD";
	}
}

IResponse*  Route::ProcessRequestMethod(const IRequest& request)
{
	method_t method = request.getMethod();
	switch (method){
	case GET:
		return (ExecuteGETMethod(request));
	case POST:
		return (ExecutePOSTMethod(request));
	case DELETE:
		return (ExecuteDELETEMethod(request));
	case HEAD:
		return (ExecuteGETMethod(request));
	}
}


std::vector<string> Route::ReadDirectory()
{
	std::vector<string> list;

	DIR* dir = opendir(path.c_str());
	if (dir)
	{
		struct dirent* entry;
		while ((entry = readdir(dir)) != NULL)
		{
			if (entry->d_type != DT_DIR) {
				list.push_back(entry->d_name);
			}
		}
		closedir(dir);
	}
	return list;
}

string	Route::GenerateDirectoryListingHtmlPage(){
	
	std::vector<string> fileList = ReadDirectory();
	
	string body = "<!DOCTYPE html>\n<html>\n<head>\n<title> Directory listing </title>\n</head>\n";
	body += "<body>\n<h1>Directory Listing</h1>\n<ul>";
	std::vector<string>::iterator it = fileList.begin();
	while (it != fileList.end()){
		string line = "<li><a href=\"" + *it + "\">" + *it + "</a></li>\n";
		body += line;
		++it;
	}
	body += "</ul>\n</body>\n";
	return body;
}

IResponse*	Route::handleDirectory(const IRequest& request){
	if (path.back() != '/')
	{
		const string& uri = request.getURI();
		IResponse * response = new Response(request.getSocket());
		response->setHeader("location", std::string(uri + "/"))
			.setHeader("connection", request.getHeader("Connection"))
			.setStatusCode(301).build();
		return response;
	}
	if (indexfile.size()){
		IResponse * response = new Response(request.getSocket());
		response->setHeader("connection", request.getHeader("Connection"))
			.setHeader("content-type", getMimeType(indexfile))
			.setStatusCode(200)
			.setBodyFile(path + indexfile)
			.build();
		return response;
	}
	if (autoindex.size()){
		string body = GenerateDirectoryListingHtmlPage();
		IResponse * response = new Response(request.getSocket());
		response->setHeader("connection", request.getHeader("Connection"))
			.setHeader("content-type", "text/html")
			.setStatusCode(200)
			.setBody(body)
			.build();
		return response;
	}
	return GenerateErrorPageResponse(request, 403);
}

int	Route::DeleteFile(){
	pid_t pid = fork();
	if (pid < 0)
	{
		perror("execve");
		return (-1);
	}
	if (pid == 0){
		const char *cmd = "/bin/rm";
		const char *args[] = {cmd, "-rf", path.c_str(), NULL};
		execve(cmd, const_cast<char **>(args), NULL);
		perror("execve");
		exit (EXIT_FAILURE);
	}
	return (utils::get_exit_status(pid));
}

IResponse*	Route::deleteDirectory(const IRequest& request){
	if (path.back() != '/')
		return GenerateErrorPageResponse(request, 409);
	if (canDeleteFile(path)){
		int ret = DeleteFile();
		if (ret != 0)
			return GenerateErrorPageResponse(request, 500); // internal server error
		return GenerateErrorPageResponse(request, 204);
	}
	return (GenerateErrorPageResponse(request, 403)); // can't delete file 
}

IResponse*	Route::handleRequestedFile(const IRequest& request){
	if (hasCGIExtension(path))
	{
		// handleCGI();
		// return ;
	}
	if (canReadFile(path)){
		IResponse * response = new Response(request.getSocket());
		response->setHeader("connection", request.getHeader("Connection"))
			.setHeader("content-type", getMimeType(path))
			.setStatusCode(200)
			.setBodyFile(path)
			.build();
		return response;
	}
	return (GenerateErrorPageResponse(request, 403)); // can't read file 
}

IResponse*  Route::ExecuteGETMethod(const IRequest& request){
	if (IsDirectory(path))
		return (handleDirectory(request));
	if (IsResourceFileExist(path))
		return (handleRequestedFile(request));
	return GenerateErrorPageResponse(request, 400);
}

IResponse*  Route::ExecutePOSTMethod(const IRequest& request)
{
	if (uploadPath.size())
	{
		upload = new Upload(request);
		string body = upload->handle(request);
		return (GenerateErrorPageResponse(request, 201));
	}
	return GenerateErrorPageResponse(request, 400);
}

IResponse*  Route::ExecuteDELETEMethod(const IRequest& request){
	if (IsDirectory(path))
		return (deleteDirectory(request));
	if (IsResourceFileExist(path)){
		if (canDeleteFile(path)){
			int ret = DeleteFile();
			if (ret != 0)
				return GenerateErrorPageResponse(request, 500); // internal server error
			return (GenerateErrorPageResponse(request, 204)); // done
		}
		return (GenerateErrorPageResponse(request, 403)); // forbiden can't delete file
	}
	if (hasCGIExtension(path))
	{
		// handleCGI();
		// return ;
	}
	return (GenerateErrorPageResponse(request, 400));
}

IResponse*  Route::handle(const IRequest& request)
{
	std::string tmp = request.getURI();
	size_t pos = tmp.find(URI);
	if (pos != std::string::npos)
		tmp.erase(0, URI.length());
	path = root + tmp;
	if (IsMethodAllowed(request.getMethod()) == false)
		return GenerateErrorPageResponse(request, 405);
	return (ProcessRequestMethod(request));
}



