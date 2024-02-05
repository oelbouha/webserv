/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MimeTypes.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysalmi <ysalmi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/24 07:41:48 by oelbouha          #+#    #+#             */
/*   Updated: 2024/02/05 15:25:43 by ysalmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "MimeTypes.hpp"

MimeTypes::MimeTypes(){}

string MimeTypes::defaultType = "text/plain";

Config* MimeTypes::mimeTypes = NULL;

const std::string& MimeTypes::getMimeType(const std::string& extension)
{
	return (mimeTypes->getInlineConfigOr(extension, defaultType));
}

void MimeTypes::setMimeTypes(Config* config)
{
	if (!mimeTypes) {

		if (config)
			mimeTypes = config;
		else 
			mimeTypes = new Config();
			
		setupNeededTypes();
		
		return ;
	}
	throw std::invalid_argument("re-setting mime types");
}

void MimeTypes::setDefault(const std::string& default_mime_type)
{
	if (!default_mime_type.empty())
		defaultType = default_mime_type;
}

void	MimeTypes::setupNeededTypes()
{
	if (!mimeTypes->hasInline("html"))
		mimeTypes->addInline("html", "text/html");
		
	if (!mimeTypes->hasInline("css"))
		mimeTypes->addInline("css", "text/css");

	if (!mimeTypes->hasInline("xml"))
		mimeTypes->addInline("xml", "text/xml");

	if (!mimeTypes->hasInline("javascript"))
		mimeTypes->addInline("javascript", "application/javascript");
		
	if (!mimeTypes->hasInline("rss+xml"))
		mimeTypes->addInline("rss+xml", "application/rss+xml");

	if (!mimeTypes->hasInline("mathml"))
		mimeTypes->addInline("mathml", "text/mathml");

	if (!mimeTypes->hasInline("plain"))
		mimeTypes->addInline("plain", "text/plain");
		
	if (!mimeTypes->hasInline("gif"))
		mimeTypes->addInline("gif", "image/gif");
		
	if (!mimeTypes->hasInline("jpeg"))
		mimeTypes->addInline("jpeg", "image/jpeg");

	if (!mimeTypes->hasInline("png"))
		mimeTypes->addInline("png", "image/png");

	if (!mimeTypes->hasInline("svg+xml"))
		mimeTypes->addInline("svg+xml", "image/svg+xml");
		
	if (!mimeTypes->hasInline("tiff"))
		mimeTypes->addInline("tiff", "image/tiff");

	if (!mimeTypes->hasInline("vnd.wap.wbmp"))
		mimeTypes->addInline("vnd.wap.wbmp", "image/vnd.wap.wbmp");
		
	if (!mimeTypes->hasInline("webp"))
		mimeTypes->addInline("webp", "image/webp");
		
	if (!mimeTypes->hasInline("x-icon"))
		mimeTypes->addInline("x-icon", "image/x-icon");
		
	if (!mimeTypes->hasInline("x-jng"))
		mimeTypes->addInline("x-jng", "image/x-jng");
		
	if (!mimeTypes->hasInline("x-ms-bmp"))
		mimeTypes->addInline("x-ms-bmp", "image/x-ms-bmp");
		
	if (!mimeTypes->hasInline("3gpp"))
		mimeTypes->addInline("3gpp", "video/3gpp");
		
	if (!mimeTypes->hasInline("mp4"))
		mimeTypes->addInline("mp4", "video/mp4");
		
	if (!mimeTypes->hasInline("mpeg"))
		mimeTypes->addInline("mpeg", "video/mpeg");

	if (!mimeTypes->hasInline("quicktime"))
		mimeTypes->addInline("quicktime", "video/quicktime");
		
	if (!mimeTypes->hasInline("webm"))
		mimeTypes->addInline("webm", "video/webm");
}