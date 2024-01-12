

#include "GetMethod.hpp"

GetMethod::GetMethod() {
	statusCode = 200;
}

GetMethod::GetMethod( const GetMethod& s ) {(void)s;}

GetMethod::~GetMethod() {}

GetMethod&	GetMethod::operator=( const GetMethod& s )
{
    (void)s;
	return (*this);
}

IResponse*  GetMethod::handle(const IRequest& request)
{
	string uri = request.getURI();
	// string root = route.getURI();
	// std::cout << "root : " << root << std::endl;
	// if (IsDirectory(uri))
	// 	handleDirectory(request);
	std::string file = "pages/index.html";
	std::string mime = "text/html";

	if (request.getURI() != "/"){
		file = "pages/";
		const std::string&    uri = request.getURI();
		file += uri.substr(uri.rfind('/'));
		if (file.rfind('.') == std::string::npos)
			mime = "text/plain";
		else {
			std::string extension = file.substr(file.rfind('.') + 1);
			std::cout << "extension: " << extension << std::endl;
			if (extension == "html")
				mime = "text/html";
			else if (extension == "jpeg")
				mime = "image/jpeg";
			else if (extension == "mp4")
				mime = "video/mp4";
		}
	}

	Response*   response = new Response(request.getSocket());

	response->setStatusCode(200)
		.setHeader("content-type", mime)
		.setHeader("connection", "keep-alive")
		.setBody("hello from webserve")
		.setBodyFile(file)
		.build();
	return response;
}