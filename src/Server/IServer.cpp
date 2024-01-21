/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IServer.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelbouha <oelbouha@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/19 22:16:26 by oelbouha          #+#    #+#             */
/*   Updated: 2024/01/21 16:00:45 by oelbouha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "src/Interfaces/IServer.hpp"
#include <sys/stat.h>

IServer::IServer(){}

IServer::~IServer(){}

void	IServer::addInlineIfNotExist(Config& route, Config& server, const string& prop){
	if (prop.empty())
		return ;
	if (route.hasInline(prop) == false)
		addInLineIfExist(route, server, prop);
}

void	IServer::setBlockIfExist(Config &config, std::vector<Config*> &block, const string& property){
	if (property.empty())
		return ;
	if (config.hasBlock(property) == true){
		block = config.getBlockConfig(property);
	}
}

void	IServer::setInlineIfExist(Config &routeConfig, string & key, const string & value){
	if (value.empty())
		return ;
	if (routeConfig.hasInline(value))
		key = routeConfig.getInlineConfig(value);
}

void	IServer::setListIfExist(Config &routeConfig, std::vector<string> &list, const string & value){
	if (value.empty())
		return ;
	if (routeConfig.hasList(value))
		list = routeConfig.getListConfig(value);
}

void	IServer::addInLineIfExist(Config& route, Config& server, const string& prop){
	if (prop.empty())
	return ;
	string property;
	if (server.hasInline(prop) == true)
	{
		property = server.getInlineConfig(prop);
		route.addInline(prop, property);
	}
}

void	IServer::addBlockIfExist(Config& route, Config& server, string prop){
	if (prop.empty())
		return ;
	if (server.hasBlock(prop) == true)
	{
		std::vector<Config*> error_page = server.getBlockConfig(prop);
		std::vector<Config*>::iterator it = error_page.begin();
		while (it != error_page.end()){
				route.addBlock(prop, *it);
			++it;
		}
	}
}

void	IServer::addListIfExist(Config& route, Config& server, const string& prop){
	if (prop.empty())
		return ;
	if (server.hasList(prop) == true)
	{
		std::vector<string> list = server.getListConfig(prop);
		route.addList(prop, list);
	}
}

bool	IServer::IsErrorCodeInErrorPages(unsigned int stsCode){
	if (error_pages.size() == 0)
		return false;
	const std::string& code = std::to_string(stsCode);
	std::vector<std::pair<string, std::vector<string> > >::iterator it = error_pages.begin();
	while (it != error_pages.end()){
		std::vector<string > codes = it->second;
		std::vector<string>::iterator pos = std::find(codes.begin(), codes.end(), code);
		if (pos != codes.end())
			return true ;
		++it;
	}
	return false;
}

IResponse*		IServer::GenerateErrorPageResponse(const IRequest& request, unsigned int stsCode){
	IResponse * response = new Response(request.getSocket());
	root = getRoot();
	if (IsErrorCodeInErrorPages(stsCode))
	{
		std::cout << " generate error page for :" << root << "  | stscode >" << stsCode << std::endl;
		const string& error_file = getStatusCodeResourceFile(stsCode);
		int pos = root.find(request.getURI());
		string path = root.erase(pos, request.getURI().length()) + error_file;
		std::cout << "error file path : " << path << std::endl;
		response->setStatusCode(stsCode)
			.setHeader("content-type", getMimeType(path))
			.setHeader("connection", request.getHeader("Connection"))
			.setBodyFile(path)
			.build();
		return response;
	}
	response->setStatusCode(stsCode)
		.setHeader("content-type", getMimeType(root))
		.setHeader("connection", request.getHeader("Connection"))
		.setBody(Response::sStatusCodes.at(stsCode))
		.build();
	return response;
}

string	IServer::getMimeType(const string& uri){
	string mime = "text/html";
	const string& extension = getExtension(uri);
	if (extension == "html")
		mime = "text/html";
	else if (extension == "jpeg")
		mime = "image/jpeg";
	else if (extension == "mp4")
		mime = "video/mp4";
	else if (extension == "css")
		mime = "text/css";
	return mime;
}

void		IServer::setupErrorPages(){
	std::pair<string, std::vector<string> > tmp;
	std::vector<Config *>::iterator it = error_page.begin();
	while (it != error_page.end()){
		Config* error_pageConfig = *it;
		if (error_pageConfig->hasInline("file") && error_pageConfig->hasList("codes"))
		{
			string  file = error_pageConfig->getInlineConfig("file");
			std::vector<string> codes = error_pageConfig->getListConfig("codes");
			tmp.first = file;
			tmp.second = codes;
			error_pages.push_back(tmp);
		}
		++it;
	}
}

string	IServer::getStatusCodeResourceFile(unsigned int stsCode){
	string file;
	const std::string& code = std::to_string(stsCode);
	
	std::vector<std::pair<string, std::vector<string> > >::iterator it = error_pages.begin();
	while (it != error_pages.end()){
		std::vector<string > codes = it->second;
		std::vector<string>::iterator pos = std::find(codes.begin(), codes.end(), code);
		if (pos != codes.end()){
			file = it->first;
			break ;
		}
		++it;
	}
	return file;
}

bool	IServer::IsDirectory(string uri){
    struct stat info;

    if (stat(uri.c_str(), &info) != 0)
        return false;
    // Check if the path corresponds to a directory
    return S_ISDIR(info.st_mode);
}

string	IServer::getExtension(string line) const{
	string extension;
	int pos = line.rfind('.');
	if (pos > 0)
		extension = line.substr(pos + 1, line.length());
	return (extension);	
}

void	IServer::printErrorPages(){
	
}