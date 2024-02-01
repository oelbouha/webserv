/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Route.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelbouha <oelbouha@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/19 18:46:25 by oelbouha          #+#    #+#             */
/*   Updated: 2024/01/30 16:52:40 by oelbouha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Route.hpp"

Route::Route(Config* config, ErrorPage& pages): error_pages(pages), config(*config) {
	redirect = config->getBlockConfigIfExist("redirect");

	if (redirect.size())
		redirect.front()->IsValidDirective("code");

	autoindex = config->getInlineConfigIfExist("autoindex");
	URI = config->getInlineConfigIfExist("uri");
	root = config->getInlineConfigIfExist("root");
	uploadPath = config->getInlineConfigIfExist("upload");
	indexfile = config->getInlineConfigIfExist("index");
	allowedMethods = config->getListConfigIfExist("allowed_methods");

	error_pages.setErrorPage(*config);
}

Route&	Route::operator=( const Route& s ) {
    (void)s;
	return (*this);
}

Route::~Route() {}

ErrorPage& 	Route::getErrorPage() const { return error_pages; }

std::vector<std::string>	Route::getAllowedMethods() const { return allowedMethods; }

Config& Route::getConfig() const { return config; }

std::string	Route::getURI() const{ return URI; }

std::string	Route::getRoot() const{ return root; }

bool	Route::hasRedirection() const { return redirect.size(); }

unsigned int Route::getStatusCode() const  { return statusCode; }

bool Route::canDeleteFile(const string& filePath) const {
    return (access(filePath.c_str(), W_OK) == 0);
}

bool Route::canReadFile(const string& filePath) const {
    return (access(filePath.c_str(), R_OK) == 0);
}

bool	Route::IsResourceFileExist(const string& filePath) const {
	return access(filePath.c_str(), F_OK) == 0;
}

bool	Route::hasCGIExtension(const string& uri) const {
	if (CGIExtensions.size() == 0)
		return false;
	std::string extension = utils::getExtension(uri);
	std::vector<string>::const_iterator it = CGIExtensions.begin();
	while (it != CGIExtensions.end()){
		if (*it == extension)
			return true;
		++it;
	}
	return false;
}

bool	Route::IsMethodAllowed(method_t m) {
	const string & method = setMethod(m);
	for(size_t i = 0; i < allowedMethods.size(); ++i){
		if (method == allowedMethods[i])
			return true;
	}
	return false;
}

const string	Route::setMethod(method_t m) {
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

IResponse*  Route::ProcessRequestMethod(const IRequest& request) {
	method_t method = request.getMethod();
	switch (method) {
	case GET:
		return (ExecuteGETMethod(request));
	case HEAD:
		return (ExecuteGETMethod(request));
	case POST:
		return (ExecutePOSTMethod(request));
	case DELETE:
		return (ExecuteDELETEMethod(request));
	}
}

std::vector<string> Route::ReadDirectory() {
	std::vector<string> list;

	DIR* dir = opendir(path.c_str());
	if (dir) {
		struct dirent* entry;
		while ((entry = readdir(dir)) != NULL) {
			if (entry->d_type != DT_DIR) 
				list.push_back(entry->d_name);
		}
		closedir(dir);
	}
	return list;
}

string	Route::BuildDirectoryListingHtmlPage() {
	
	std::vector<string> fileList = ReadDirectory();
	
	string body = "<h1> Directory Listing </h1> <ul>";
	std::vector<string>::iterator it = fileList.begin();

	while (it != fileList.end()) {
		string line = "<li> <a href=\"" + *it + "\">" + *it + "</a> </li> ";
		body += line;
		++it;
	}
	body += "</ul>";
	return body;
}

IResponse*	Route::handleDirectory(const IRequest& request) {
	if (path.back() != '/')
	{
		const string& uri = request.getURI();
		IResponse * response = new Response(request.getSocket());
		response->setHeader("location", std::string(uri + "/"))
			.setHeader("connection", request.getHeader("Connection"))
			.setStatusCode(301).build();
		return response;
	}
	else if (indexfile.size()){
		IResponse * response = new Response(request.getSocket());
		response->setHeader("connection", request.getHeader("Connection"))
			.setStatusCode(200)
			.setBodyFile(path + indexfile)
			.build();
		return response;
	}
	else if (autoindex.size()){
		string body = BuildDirectoryListingHtmlPage();
		IResponse * response = new Response(request.getSocket());
		response->setHeader("connection", request.getHeader("Connection"))
			.setHeader("content-type", "text/html")
			.setStatusCode(200)
			.setBody(body)
			.build();
		return response;
	}
	else
		statusCode = 403;
	return (Helper::BuildResponse(request, *this));
}

int	Route::DeleteFile() {
	pid_t pid = fork();
	if (pid < 0) {
		perror("execve");
		return (-1);
	}
	if (pid == 0) {
		const char *cmd = "/bin/rm";
		const char *args[] = {cmd, "-rf", path.c_str(), NULL};
		execve(cmd, const_cast<char **>(args), NULL);
		perror("execve");
		exit (EXIT_FAILURE);
	}
	return (utils::get_exit_status(pid));
}

IResponse*	Route::deleteDirectory(const IRequest& request) {
	if (path.back() != '/')
		statusCode = 409;
	if (canDeleteFile(path)) {
		int ret = DeleteFile();
		if (ret != 0)
			statusCode = 500;
		else
			statusCode = 204;
	}
	return (Helper::BuildResponse(request, *this));
}

IResponse*	Route::handleRequestedFile(const IRequest& request) {
	if (hasCGIExtension(path))
	{
		// handleCGI();
		// return ;
	}
	if (canReadFile(path)) {
		IResponse * response = new Response(request.getSocket());
		response->setHeader("connection", request.getHeader("Connection"))
			.setStatusCode(200)
			.setBodyFile(path)
			.build();
		return response;
	}
	else
		statusCode = 403; // cant read file
	return (Helper::BuildResponse(request, *this));
}

IResponse*  Route::ExecuteGETMethod(const IRequest& request) {
	std::cout << "------------- GEt Method --------------\n";
	if (utils::IsDirectory(path))
		return (handleDirectory(request));
	else if (IsResourceFileExist(path))
		return (handleRequestedFile(request));
	statusCode = 400; // not found
	return (Helper::BuildResponse(request, *this));
}

IResponse*  Route::ExecutePOSTMethod(const IRequest& request) {
	std::cout << "------------- POST Method --------------\n";
	if (uploadPath.size()) {
		Upload upload(request);
		std::string buff;
		IClientSocket &client = request.getSocket();
		upload.buff += client.readAll();
		while(true) {
			if (upload.IsDone())
				break ;
			buff = upload.handle(request);
		}
		std::cout << "*************************************************************\n";
		std::cout << buff << std::endl;
		std::cout << "*************************************************************\n";
		
		statusCode = 201;
	}
	else 
		statusCode = 400; 
	return (Helper::BuildResponse(request, *this));
}

IResponse*  Route::ExecuteDELETEMethod(const IRequest& request) {
	if (utils::IsDirectory(path))
		return (deleteDirectory(request));
	else if (IsResourceFileExist(path)) {
		if (canDeleteFile(path))
		{
			int ret = DeleteFile();
			if (ret != 0)
				statusCode = 500;
			else
				statusCode = 204;
		}
		statusCode = 403;
	}
	else if (hasCGIExtension(path))
	{
		// handleCGI();
		// return ;
	}
	else
		statusCode = 400;
	return (Helper::BuildResponse(request, *this));
}

IResponse*  Route::handle(const IRequest& request) {
	std::string uri = URI;
	if (uri.back() == '/')
		uri.erase(uri.length() - 1);
	
	std::string tmp = request.getURI();
	size_t pos = tmp.find(uri);
	if (pos != std::string::npos)
		tmp.erase(0, uri.length());
	
	path = root + tmp;
	std::cout << "path >" << path << std::endl;
	if (IsMethodAllowed(request.getMethod()) == false) {
		statusCode = 405;
		return (Helper::BuildResponse(request, *this));
	}
	return (ProcessRequestMethod(request));
}



