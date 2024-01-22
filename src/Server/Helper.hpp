/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Helper.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelbouha <oelbouha@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 21:59:44 by oelbouha          #+#    #+#             */
/*   Updated: 2024/01/22 22:53:11 by oelbouha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */



#ifndef EXTENDER_HPP
#define EXTENDER_HPP

#include <iostream>

#include "src/Response/Response.hpp"
#include "src/Request/Request.hpp"
#include "src/DataTypes/Config.hpp"

class Helper{
	Helper();
	public:
		static 	vector<Config*>		getBlockConfigIfExist(Config& config, const string& prop);
		static  vector<string>		getListConfigIfExist(Config& config, const string& prop);
		static  const string		getInlineConfigIfExist(Config& config, const string& prop);
};

#endif


// class    MimeTypes {

//     static Config*        mimeTypes = NULL;
//     static std::string    defaultMimeType = "application/";

//     MimeTypes(){};


// public:
//     static void            setMimeTypes(Config* config);
//     static void            setDefaultMimeType(const std::string& MimeType);
//     static std::string    getMimeType(const std::string& extention);
// };

// class    ErrorPages {

//     std::map<string, string>       error_pages;

//     ErrorPages(){};


// public:
//     void            setErrorPage(Config* config);
//     std::string    getErrorPagePath(unsigned int stsCode);
// };

// cluster
// if (clusterConfig->hasBlockConfig())
//     MimeTypes::setMimeTypes(clusterConfig->getBlockConfig("mime_types").front())
// if (clusterConfig->hasInlineConfig("default_mime_type"))
//     MimeType::setDefaultMimeType(clusterConfig->getInlineConfig("default_mime_type"))