/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Upload.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysalmi <ysalmi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/19 18:46:11 by oelbouha          #+#    #+#             */
/*   Updated: 2024/03/28 22:56:14 by ysalmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Upload.hpp"

Upload::Upload(IRequest *request, const string& upload_path) : 
	request(request),
	firstRead(true),
	is_done(false),
	upload_dir(upload_path),
	count(0),
	client(NULL)
{
	if (upload_dir[upload_dir.length() - 1] != '/')
		upload_dir += "/";

	std::string type = request->getHeader("content-type");
	int pos = type.rfind("=");
	boundry = "--" + type.substr(pos + 1, type.length());
	start = "Content-Disposition: form-data; name=\"";
	body = boundry;

	handle();
}

Upload::Upload(const Upload& obj){ (void)obj; }

Upload& Upload::operator=(const Upload& obj) {
	(void)obj;
	return *this;
}

Upload::~Upload()
{
	delete request;
}



// bool 		Upload::search(const string& buff, const string & line) {
// 	return (buff.find(line) >= 0);
// }

void		Upload::setErrorPages(const ErrorPages* err_pages)
{ error_pages = err_pages; }

bool 		Upload::done() const
{
	Logger::debug ("is done: ")(is_done?"Yes":"No").flush();
	return is_done;
}

std::string	Upload::getNextTmpName()
{
    static int  n;

    std::stringstream ss;
    ss << std::setw(11) << std::setfill('0') << n;
    n++;
    return ss.str();
}

void		Upload::createTmpFile()
{
	name = getFieldName("name=\"", "\";");
	file_name = getFieldName("filename=\"", "\"");
	file_content_type = getFieldName("Content-Type:", "\r\n");
	utils::trim_spaces(file_content_type);
	
	file_path = upload_dir + utils::to_string(time(0)) + utils::to_string(count++);
	file_stream.open(file_path);
	if (file_stream.bad())
		throw RequestException(RequestException::FORBIDEN);
	buff.erase(0, buff.find("\r\n\r\n") + 4);

	size_t pos = buff.find(boundry);
	if (pos != std::string::npos)
	{		
		file_stream << buff.substr(0, pos - 2);
		file_stream.close();
		
		buff.erase(0, pos + boundry.length());
		
		body += std::string("\r\n") + start + name + "_name\"\r\n\r\n" + file_name + "\r\n" + boundry + "\r\n";
		body += start + name + "_content-type\"\r\n\r\n" + file_content_type + "\r\n" + boundry + "\r\n";
		body += start + name + "_path\"\r\n\r\n" + file_path + "\r\n" + boundry;
	}
	else
	{
		file_stream << buff;
		buff.clear();
	}
}

void		Upload::buildUploadRequest()
{
	size_t pos = buff.find(boundry);
	if (pos != std::string::npos)
	{		
		file_stream << buff.substr(0, pos - 2);
		file_stream.close();
		
		buff.erase(0, pos + boundry.length());
		
		body += std::string("\r\n") + start + name + "_name\"\r\n\r\n" + file_name + "\r\n" + boundry + "\r\n";
		body += start + name + "_content_type\"\r\n\r\n" + file_content_type + "\r\n" + boundry + "\r\n";
		body += start + name + "_path\"\r\n\r\n" + file_path + "\r\n" + boundry;
	}
	else
	{
		file_stream << buff;
		buff.clear();
	}
}

std::string Upload::getFieldName(const std::string& name, const std::string& del)
{
	int pos = buff.find(name);
	int end = buff.find(del, pos + name.length());
	return  (buff.substr(pos + name.length(), end - pos - name.length()));
}

void		Upload::handle()
{
	buff += request->read();

	if (firstRead)
	{
		if (std::strncmp(boundry.c_str(), buff.c_str(), boundry.length()) != 0)
			Logger::debug (" not a valid boundry :")(buff.substr(0, boundry.length())).flush();
		buff.erase(0, boundry.length());
		firstRead = false;
	}
	
	while (!buff.empty())
	{
		if (buff.find("\r\n\r\n") != std::string::npos)
		{
			if (buff.find("Content-Type") > buff.find("\r\n\r\n"))
			{
				if (buff.find(boundry) == std::string::npos)
					break ;
					
				string content = buff.substr(0, buff.find(boundry) + boundry.length());
				body += content;
				buff.erase(0, content.length());
			}
			else 
				createTmpFile();
		}
		else if (file_stream.is_open())
			buildUploadRequest();
		
		if (buff == "--\r\n")
		{
			is_done = true;
			body += "--\r\n";
			buff.clear();
			break;
		}
	}
}

int			Upload::getSocketFd() const
{
	return request->getSocketFd();
}

Request*	Upload::getRequest()
{
	Request *req = new BufferRequest(*static_cast<Request*>(request), body);
    req->setHeader("x-upload", "false");
	return req;
}

IResponse*	Upload::buildErrorPage(int code) const
{
	return error_pages->build(*request, code);
}