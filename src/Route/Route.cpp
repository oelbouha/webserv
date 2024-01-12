

#include "Route.hpp"

Route::Route(const Config* config){ 
	hasRedirection = false;
	URI = config->getInlineConfig("uri");
	root = config->getInlineConfig("root");
	indexfile = config->getInlineConfig("index");
	allowedMethods = config->getListConfig("allowed_methods");
}

std::string	Route::getURI() const{ return URI; }

bool	Route::IsExist(string& uri) const{
	std::string path = root + uri;
	int fd = ::open(path.c_str(), O_RDONLY);
	if (fd < 0)
		return false;
	return true;
}

bool	Route::isCGI(string& uri) const{

	std::vector<string> cgi;
	cgi.push_back(".php");
	cgi.push_back(".py");

	std::string extension = utils::getExtension(uri);
	std::vector<string>::iterator it = cgi.begin();
	while (it != cgi.end()){
		if (*it == extension)
			return true;
	}
	return true;
}

std::vector<std::string>	Route::getAllowedMethods() const{ return allowedMethods; }

bool	Route::hasRedirect() const {return hasRedirection; }

bool	Route::IsMethodAllowed() const {
	for(size_t i = 0; i < allowedMethods.size(); ++i)
	{
		if (method == allowedMethods[i])
			return true;
	}
	return false;
}

IResponse*	Route::handleDirectory(const IRequest& request){
	IResponse * res = new Response(request.getSocket());

	string uri = request.getURI();
	if (uri[uri.length() - 1] != '/')
	{
		res->setHeader("location", uri += "/")
		.setHeader("connection", "Keep-Alive")
		.setStatusCode(301).build();
	}
	res->setHeader("connection", "Keep-Alive")
	.setHeader("content-type", "text/html")
	.setStatusCode(200)
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
	string mime = "text/html";
	string uri = request.getURI();
	if (isCGI(uri))
	{
		// handleCGI();
		// return ;
	}
	std::string extension = utils::getExtension(uri);
	if (extension == "html")
		mime = "text/html";
	else if (extension == "jpeg")
		mime = "image/jpeg";
	else if (extension == "mp4")
		mime = "video/mp4";
	else if (extension == "css")
		mime = "text/css";

	std::string file = root + uri;
	res->setStatusCode(200)
		.setHeader("content-type", mime)
		.setHeader("connection", "keep-alive")
		.setBody("hello from webserve")
		.setBodyFile(file)
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

Route::Route( const Route& s ) {(void)s;}

Route::~Route() {}

Route&	Route::operator=( const Route& s )
{
    (void)s;
	return (*this);
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
	std::string mime = "text/html";
	string requestUri = request.getURI();
	string path = root + requestUri;

	if (utils::IsDirectory(path))
		return (handleDirectory(request));
	if (IsExist(path) == false)
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
	std::string mime = "text/html";
	string uri = request.getURI();
	string path = root + uri;
	// if (route suport upload)
	// {
	// 	// upload file
	// }
	// if (utils::IsDirectory(path))
	// 	return (handleDirectory(request));
	if (IsExist(path) == false)
	{
		response = new Response(request.getSocket());
		response->setStatusCode(400)
			.setHeader("content-type", "text/html")
			.setHeader("connection", "close")
			.setBody("400 Not Found")
			.build();
		return response;
	}
	if (isCGI(uri))
	{
		// handleCGI();
		// return ;
	}
	std::string file = root + uri;
	response->setStatusCode(403)
		.setHeader("content-type", mime)
		.setHeader("connection", "close")
		.setBody("403 forbidden")
		.setBodyFile(file)
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
	if (IsExist(path) == false)
	{
		response = new Response(request.getSocket());
		response->setStatusCode(400)
			.setHeader("content-type", "text/html")
			.setHeader("connection", "close")
			.setBody("400 Not Found")
			.build();
		return response;
	}
	if (isCGI(uri))
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
		// method not implemented
		// if method not allowed return 405 method not allowed 
	}
	IResponse* response =  WhichMethodTouse(request);
	return response;
}
