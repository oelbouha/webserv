

#include "Route.hpp"

Route::Route(const Config* config){ 
	hasRedirection = false;
	Default = false;
	URI = config->getInlineConfig("uri");
	root = config->getInlineConfig("root");
	indexfile = config->getInlineConfig("index");
	allowedMethods = config->getListConfig("allowed_methods");
}

std::vector<std::string>	Route::getAllowedMethods() const{ return allowedMethods; }

bool	Route::IsDefault() const { return Default; }

std::string	Route::getURI() const{ return URI; }

bool	Route::hasRedirect() const {return hasRedirection; }

Route::Route( const Route& s ) {(void)s;}

Route&	Route::operator=( const Route& s ){
    (void)s;
	return (*this);
}

Route::~Route() {}

bool	Route::IsResourceFileExist(const string& uri) const{
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

bool	Route::hasCGIExtension(const string& uri) const{
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
	const string& uri = request.getURI();
	if (uri.back() != '/')
	{
		
		res->setHeader("location", std::string(uri + "/"))
		.setHeader("connection", request.getHeader("Connection"))
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
	const string& uri = request.getURI();
	if (uri.back() != '/')
	{
		res->setStatusCode(409)
			.setHeader("connection", request.getHeader("Connection"))
			.setBody("409 conflict")
			.build();
		return res;
	}
	std::string cmd = "rm -rf " + std::string(root + uri);
	if (std::system(cmd.c_str()) < 0)
	{
		std::cout << "system error ..." << std::endl;
	}
	res->setHeader("connection", request.getHeader("Connection"))
		.setStatusCode(204)
		.setBody("204 No Content")
		.build();
	return res;
}

IResponse*	Route::handleRequestedFile(const IRequest& request){
	IResponse * res = new Response(request.getSocket());
	const string &uri = request.getURI();
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
}

IResponse*  Route::ProcessRequestMethod(const IRequest& request)
{
	IResponse * response = NULL;
	if (method == "GET")
		response = ExecuteGETMethod(request);
	else if (method == "POST")
		response = ExecutePOSTMethod(request);
	else if (method == "DELETE")
		response = ExecuteDELETEMethod(request);
	else if (method == "HEAD")
		response = ExecuteGETMethod(request);
	return response;
}

IResponse*  Route::ExecuteGETMethod(const IRequest& request)
{
	IResponse * response = NULL;
	const string& requestUri = request.getURI();
	const string& path = root + requestUri;
	if (utils::IsDirectory(path))
		return (handleDirectory(request));
	if (IsResourceFileExist(path) == false)
	{
		response = new Response(request.getSocket());
		response->setStatusCode(400)
			.setHeader("content-type", getMimeType(requestUri))
			.setHeader("connection", request.getHeader("Connection"))
			.setBody("400 Not Found")
			.build();
		return response;
	}
	return (handleRequestedFile(request));
}

IResponse*  Route::ExecutePOSTMethod(const IRequest& request)
{
	IResponse * response = NULL;
	const string& uri = request.getURI();
	const string& path = root + uri;
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

IResponse*  Route::ExecuteDELETEMethod(const IRequest& request)
{
	IResponse * response = NULL;
	const string& uri = request.getURI();
	const string& path = root + uri;
	if (utils::IsDirectory(path))
		return (deleteDirectory(request));
	response = new Response(request.getSocket());
	if (IsResourceFileExist(path) == false)
	{
		response->setStatusCode(400)
			.setHeader("content-type", "text/html")
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
	const string& cmd = "rm -rf " + path;
	if (std::system(cmd.c_str()) < 0)
	{
		std::cout << "system error ..." << std::endl;
	}
	response->setStatusCode(204)
		.setHeader("connection", request.getHeader("Connection"))
		.setBody("204 No Content")
		.setBodyFile(path)
		.build();
	return response;
}

IResponse*  Route::handle(const IRequest& request)
{
	setMethod(request.getMethod());
	if (!IsMethodAllowed())
	{
		IResponse *response = new Response(request.getSocket());
		response->setStatusCode(405)
			.setHeader("content-type", "text/html")
			.setHeader("connection", request.getHeader("Connection"))
			.setBody("405 Method Not Implemented")
			.build();
		return response;
	}
	return (ProcessRequestMethod(request));
}
