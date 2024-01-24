/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Mimetypes.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelbouha <oelbouha@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/24 07:41:48 by oelbouha          #+#    #+#             */
/*   Updated: 2024/01/24 12:47:05 by oelbouha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "MimeTypes.hpp"

MimeTypes::MimeTypes(){}

string MimeTypes::defaultMimeType = "text/plain";

std::map<string, string> MimeTypes::Mime_Types = MimeTypes::initDefaultMimeTypes();

std::string MimeTypes::getMimeType(const std::string& extension){
	return Mime_Types.at(extension);
}

std::string MimeTypes::getDefaultMimeType(){
	return defaultMimeType;
}

void	MimeTypes::setDefaultMimeType(const std::string& MimeType){
	defaultMimeType = MimeType;
}

bool MimeTypes::hasMimeType(const std::string& extension){
	try{
		Mime_Types.at(extension);
		return true;
	}
	catch(...){
		return false;
	}
}

void MimeTypes::setMimeTypes(Config* config){
	std::vector<Config*> mimeTypes = config->getBlockConfigIfExist("mime_types");
	
	if (mimeTypes.size() == 0)
		return ;
		

	std::vector<Config*>::iterator it = mimeTypes.begin();
	while (it != mimeTypes.end())
	{
		Config *mimetypeConfig = *it;
		
		if (mimetypeConfig->hasInline("default_mime"))
			defaultMimeType = mimetypeConfig->getInlineConfigIfExist("default_mime");
		// const string& extension = mimetypeConfig->getInlineConfigIfExist();
		// Mime_Types[];
		++it;
	}
	
}

std::map<string, string>	MimeTypes::initDefaultMimeTypes(){
	std::map<string, string> mimetypes;
	
	mimetypes["html"] = "text/html";
	mimetypes["css"] = "text/css";
	mimetypes["xml"] = "text/xml";
	mimetypes["javascript"] = "application/javascript";
	mimetypes["rss+xml"] = "application/rss+xml";
	mimetypes["mathml"] = "text/mathml";
	mimetypes["plain"] = "text/plain";
	
	// images mimetypes
	mimetypes["gif"] = "image/gif";
	mimetypes["jpeg"] = "image/jpeg";
	mimetypes["png"] = "image/png";
	mimetypes["svg+xml"] = "image/svg+xml";
	mimetypes["tiff"] = "image/tiff";
	mimetypes["vnd.wap.wbmp"] = "image/vnd.wap.wbmp";
	mimetypes["webp"] = "image/webp";
	mimetypes["x-icon"] = "image/x-icon";
	mimetypes["x-jng"] = "image/x-jng";
	mimetypes["x-ms-bmp"] = "image/x-ms-bmp";

	// video mimetypes
	mimetypes["3gpp"] = "video/3gpp";
	mimetypes["mp4"] = "video/mp4";
	mimetypes["mpeg"] = "video/mpeg";
	mimetypes["quicktime"] = "video/quicktime";
	mimetypes["webm"] = "video/webm";

	return mimetypes;
}
