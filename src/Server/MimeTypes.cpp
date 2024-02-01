/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Mimetypes.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelbouha <oelbouha@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/24 07:41:48 by oelbouha          #+#    #+#             */
/*   Updated: 2024/01/24 19:10:21 by oelbouha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "MimeTypes.hpp"

MimeTypes::MimeTypes(){}

string MimeTypes::defaultMimeType = "text/plain";

Config* MimeTypes::mimeTypesConfig = NULL;

std::string MimeTypes::getMimeType(const std::string& extension){
	if (mimeTypesConfig->hasInline(extension))
		return (mimeTypesConfig->getInlineConfigIfExist(extension));
	else
		return defaultMimeType;
}

void MimeTypes::setMimeTypes(Config* config){
	mimeTypesConfig = config;
	if (config->hasBlock("default_mime"))
		defaultMimeType = config->getInlineConfig("default_mime");
}

