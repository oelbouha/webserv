

#include "Route.hpp"

Route::Route(const Config* config){ 
	hasRedirection = false;
	URI = config->getInlineConfig("uri");
	root = config->getInlineConfig("root");
	indexfile = config->getInlineConfig("index");
	allowedMethods = config->getListConfig("allowed_methods");
}

std::vector<std::string>	Route::getAllowedMethods() const{ return allowedMethods; }

std::string	Route::getURI() const{ return URI; }

bool	Route::hasRedirect() const {return hasRedirection; }

Route::Route( const Route& s ) {(void)s;}

Route&	Route::operator=( const Route& s ){
    (void)s;
	return (*this);
}

Route::~Route() {}

bool	Route::IsResourceFileExist(string& uri) const{
	int fd = ::open(uri.c_str(), O_RDONLY);
	if (fd < 0)
		return false;
	return true;
}

Route&	Route::setStatusCode(unsigned int sts){
	statusCode = sts;
	return *this;
}

Route&	Route::setResponseHeader(const string& key, const string& value){
	resHeader[key] = value;
	return (*this);
}

const string	Route::getMimeType(const string& uri){
	string mime = "text/html";
	std::string extension = utils::getExtension(uri);
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

bool	Route::hasCGIExtension(string& uri) const{
	std::vector<string> cgiExtensions;
	cgiExtensions.push_back(".php");
	cgiExtensions.push_back(".py");

	std::string extension = utils::getExtension(uri);
	std::vector<string>::iterator it = cgiExtensions.begin();
	while (it != cgiExtensions.end()){
		if (*it == extension)
			return true;
		++it;
	}
	return false;
}

bool	Route::IsMethodAllowed() const {
	for(size_t i = 0; i < allowedMethods.size(); ++i){
		if (method == allowedMethods[i])
			return true;
	}
	return false;
}

IResponse*	Route::handleDirectory(const IRequest& request){
	IResponse * res = new Response(request.getSocket());
	string uri = request.getURI();
	if (uri.back() != '/')
	{
		res->setHeader("location", uri += "/")
		.setHeader("connection", "Keep-Alive")
		.setStatusCode(301).build();
		return res;
	}
	res->setStatusCode(200)
		.setHeader("connection", request.getHeader("Connection"))
		.setHeader("content-type", getMimeType(uri))
		.setBodyFile(root + uri + indexfile)
		.build();
	return res;
}

IResponse*	Route::deleteDirectory(const IRequest& request){
	IResponse * res = new Response(request.getSocket());

	string uri = request.getURI();
	if (uri[uri.length() - 1] != '/')
	{
		res->setHeader("connection", "close")
		.setStatusCode(409)
		.setBody("409 conflict")
		.build();
	}
	std::string cmd = "m -rf " + std::string(root + uri);
	int ret = std::system(cmd.c_str());
	if (ret < 0)
	{
		// system error 
	}
	res->setHeader("connection", "close")
		.setStatusCode(204)
		.setBody("204 No Content")
		.build();
	return res;
}

IResponse*	Route::handleFile(const IRequest& request){
	IResponse * res = new Response(request.getSocket());
	string uri = request.getURI();
	if (hasCGIExtension(uri))
	{
		// handleCGI();
		// return ;
	}
	res->setStatusCode(200)
		.setHeader("content-type", getMimeType(uri))
		.setHeader("connextion", request.getHeader("Connection"))
		.setBody("hello from webserve")
		.setBodyFile(root + uri)
		.build();
	return res;
}

void	Route::setMethod(method_t m){
	if (m == GET)
		method = "GET";
	else if (m == HEAD)
		method = "HEAD";
	else if (m == DELETE)
		method = "DELETE";
	else if (m == POST)
		method = "POST";
	else
		method = "NOTIMPLEMENTED";
}


IResponse*  Route::WhichMethodTouse(const IRequest& request)
{
	IResponse * response = NULL;
	if (method == "GET")
		response = HandleGET(request);
	else if (method == "POST")
		response = HandleGET(request);
	else if (method == "DELETE")
		response = HandleGET(request);
	else if (method == "HEAD")
		response = HandleGET(request);
	return response;
}

IResponse*  Route::HandleGET(const IRequest& request)
{
	IResponse * response = NULL;
	string requestUri = request.getURI();
	string path = root + requestUri;
	if (utils::IsDirectory(path))
		return (handleDirectory(request));
	if (IsResourceFileExist(path) == false)
	{
		response = new Response(request.getSocket());
		response->setStatusCode(400)
			.setHeader("content-type", "text/html")
			.setHeader("connection", "close")
			.setBody("400 Not Found")
			.build();
		return response;
	}
	response = handleFile(request);
	return response;
}

IResponse*  Route::HandlePOST(const IRequest& request)
{
	IResponse * response = NULL;
	string uri = request.getURI();
	string path = root + uri;
	// if (route suport upload)
	// {
	// 	// upload file
	// }
	// if (utils::IsDirectory(path))
	// 	return (handleDirectory(request));
	response = new Response(request.getSocket());
	if (IsResourceFileExist(path) == false)
	{
		response->setStatusCode(400)
			.setHeader("content-type", getMimeType(uri))
			.setHeader("connection", request.getHeader("Connection"))
			.setBody("400 Not Found")
			.build();
		return response;
	}
	if (hasCGIExtension(uri))
	{
		// handleCGI();
		// return ;
	}
	response->setStatusCode(403)
		.setHeader("content-type", getMimeType(uri))
		.setHeader("connection", request.getHeader("Connection"))
		.setBody("403 forbidden")
		.setBodyFile(path)
		.build();
	return response;
}

IResponse*  Route::HandleDELETE(const IRequest& request)
{
	IResponse * response = NULL;
	string uri = request.getURI();
	string path = root + uri;

	if (utils::IsDirectory(path))
		return (deleteDirectory(request));
	response = new Response(request.getSocket());
	if (IsResourceFileExist(path) == false)
	{
		response->setStatusCode(400)
			.setHeader("content-type", "text/html")
			.setHeader("connection", "close")
			.setBody("400 Not Found")
			.build();
		return response;
	}
	if (hasCGIExtension(uri))
	{
		// handleCGI();
		// return ;
	}
	std::string file = root + uri;
	string cmd = "rm -rf " + file;
	int ret = std::system(cmd.c_str());
	if (ret < 0)
	{
		// system error 
	}
	response->setStatusCode(204)
		.setHeader("connection", "close")
		.setBody("204 No Content")
		.setBodyFile(file)
		.build();
	return response;
}

IResponse*  Route::handle(const IRequest& request)
{
	setMethod(request.getMethod());
	if (method == "NOTIMPLEMENTED")
	{
		IResponse *response = new Response(request.getSocket());
		response->setStatusCode(405)
			.setHeader("content-type", "text/html")
			.setHeader("connection", "close")
			.setBody("405 Method Not Implemented")
			.build();
		return response;
	}
	IResponse* response =  WhichMethodTouse(request);
	return response;
}
