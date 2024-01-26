/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Upload.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelbouha <oelbouha@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/19 18:46:11 by oelbouha          #+#    #+#             */
/*   Updated: 2024/01/26 22:55:12 by oelbouha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "Upload.hpp"

Upload::Upload(const IRequest& request) {
	const string & type = request.getHeader("Content-Type");
	int pos = type.rfind("=");
	boundry = "--" + type.substr(pos + 1, type.length());
	firstRead = true;
	done = false;
	upload_dir = request.getURI() + "/";
	body = boundry + "\r\n";
	start = "Content-Disposition: form-data; name=\"";
}

Upload::Upload(const Upload& obj){
	(void)obj;
}

bool Upload::IsDone() const { return done; }

Upload& Upload::operator=(const Upload& obj) {
	(void)obj;
	return *this;
}

Upload::~Upload(){}


bool Upload::search(const string& buff, const string & line) {
	int pos = buff.find(line);
	if (pos < 0)
		return false;
	return true;
}

string getNextTmpName()
{
    static int  n;

    std::stringstream ss;
    ss << std::setw(11) << std::setfill('0') << n;
    n++;
    return ss.str();
}

std::string Upload::getFieldName() {
	const string & name = "name=\"";
	int pos = buff.find(name);
	int end = buff.find("\";", pos + name.length());
	const string &content = buff.substr(pos + name.length(), end - pos - name.length());
	return (content);
}

std::string Upload::getFileName() {
	const string & name = "filename=\"";
	int pos = buff.find(name);
	int end = buff.find("\"", pos + name.length());
	const string &content = buff.substr(pos + name.length(), end - pos - name.length());
	return (content);
}

std::string Upload::getContentType() {
	const string & name = "Content-Type: ";
	int pos = buff.find(name);
	int end = buff.find("\r\n", pos + name.length());
	const string &content = buff.substr(pos + name.length(), end - pos - name.length());
	return (content);
}

std::string  Upload::handle(const IRequest& __unused request) {
	
	if (firstRead)
	{
		if (std::strncmp(boundry.c_str(), buff.c_str(), boundry.length()) != 0)
		{
			std::cout << " not a valid boundry : " << std::endl;
		}
		buff.erase(0, boundry.length());
		firstRead = false;
	}
	if (search(buff, "\r\n\r\n"))
	{
		if (search(buff, "Content-Type") == false)
		{
			if (search(buff, boundry)) {
				string content = buff.substr(0, buff.find(boundry) + boundry.length());
				body += content;
				buff.erase(0, content.length());
			}
		}
		else {
			name = getFieldName();
			file_name = getFileName();
			file_content_type = getContentType();
			file_path = upload_dir + getNextTmpName();
			string path = "/Users/oelbouha/Desktop/web" + file_path;
			file_stream.open(path);
			buff.erase(0, buff.find("\r\n\r\n") + 4);
		}
	}
	if (file_stream.is_open())
	{
		size_t pos = buff.find(boundry);
		if (pos != std::string::npos)
		{
			file_stream << buff.substr(0, pos);
			file_stream.close();
			buff.erase(0, pos + boundry.length());
			body += std::string("\r\n") + start + name + "_name\"\r\n\r\n" + file_name + "\r\n" + boundry + "\r\n";
			body += start + name + "_content-type\"\r\n\r\n" + file_content_type + "\r\n" + boundry + "\r\n";
			body += start + name + "_path\"\r\n\r\n" + file_path + "\r\n" + boundry;
		}
		else {
			file_stream << buff;
			buff.clear();
		}
	}
	else if (buff == "--\r\n") {
		done = true;
		body += "--\r\n";
	}
	return body;
}
