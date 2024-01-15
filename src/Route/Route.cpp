

#include "Route.hpp"

Route::Route(const Config* config){ 
	hasRedirect = false;
	URI = config->getInlineConfig("uri");
	root = config->getInlineConfig("root");
	indexfile = config->getInlineConfig("index");
	allowedMethods = config->getListConfig("allowed_methods");
}

std::vector<std::string>	Route::getAllowedMethods() const{ return allowedMethods; }

std::string	Route::getURI() const{ return URI; }

bool	Route::hasRedirection() const {return hasRedirect; }

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

const string	Route::getMimeType(const string& uri){
	string mime = "text/html";
	const string& extension = utils::getExtension(uri);
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

bool	Route::IsMethodAllowed(method_t m){
	const string & method = setMethod(m);
	std::cout << "method -> " << method << std::endl;
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
	return "";
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
	return NULL;
}



IResponse*	Route::handleDirectory(const IRequest& request){
	IResponse * response = new Response(request.getSocket());
	const string& uri = request.getURI();
	if (root.back() != '/')
	{
		response->setHeader("location", std::string(uri + "/"))
		.setHeader("connection", request.getHeader("Connection"))
		.setStatusCode(301).build();
		return response;
	}
	response->setStatusCode(200)
		.setHeader("connection", request.getHeader("Connection"))
		.setHeader("content-type", getMimeType(root))
		.setBodyFile(root + indexfile)
		.build();
	return response;
}

IResponse*	Route::deleteDirectory(const IRequest& request){
	IResponse * res = new Response(request.getSocket());
	if (root.back() != '/')
	{
		res->setStatusCode(409)
			.setHeader("connection", request.getHeader("Connection"))
			.setBody("409 conflict")
			.build();
		return res;
	}
	std::string cmd = "rm -rf " + std::string(root);
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
	if (hasCGIExtension(root))
	{
		// handleCGI();
		// return ;
	}
	res->setStatusCode(200)
		.setHeader("content-type", getMimeType(root))
		.setHeader("connextion", request.getHeader("Connection"))
		.setBody("hello from webserve")
		.setBodyFile(root)
		.build();
	return res;
}

IResponse*  Route::ExecuteGETMethod(const IRequest& request)
{
	if (utils::IsDirectory(root))
		return (handleDirectory(request));
	if (IsResourceFileExist(root))
		handleRequestedFile(request);
	IResponse * response = new Response(request.getSocket());
	response->setStatusCode(400)
		.setHeader("content-type", getMimeType(root))
		.setHeader("connection", request.getHeader("Connection"))
		.setBody("400 Not Found")
		.build();
	return response;
}

IResponse*  Route::ExecutePOSTMethod(const IRequest& request)
{
	// if (route suport upload)
	// {
	// 	// upload file
	// }
	// if (utils::IsDirectory(path))
	// 	return (handleDirectory(request));
	IResponse * response = new Response(request.getSocket());
	if (IsResourceFileExist(root) == false)
	{
		response->setStatusCode(400)
			.setHeader("content-type", getMimeType(root))
			.setHeader("connection", request.getHeader("Connection"))
			.setBody("400 Not Found")
			.build();
		return response;
	}
	if (hasCGIExtension(root))
	{
		// handleCGI();
		// return ;
	}
	response->setStatusCode(403)
		.setHeader("content-type", getMimeType(root))
		.setHeader("connection", request.getHeader("Connection"))
		.setBody("403 forbidden")
		.setBodyFile(root)
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
	root += request.getURI();
	// std::cout << "URI --> " << root << std::endl;
	if (IsMethodAllowed(request.getMethod()))
		return (ProcessRequestMethod(request));
	else if (hasRedirection())
	{
		// handle redirection 
	}
	IResponse *response = new Response(request.getSocket());
	response->setStatusCode(405)
		.setHeader("content-type", "text/html")
		.setHeader("connection", request.getHeader("Connection"))
		.setBody("405 Method Not Allowed")
		.build();
	return response;
}
