/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MimeTypes.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelbouha <oelbouha@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/23 17:51:14 by oelbouha          #+#    #+#             */
/*   Updated: 2024/01/24 09:57:44 by oelbouha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MIMETPE_HPP
#define MIMETPE_HPP

#include <iostream>
#include "src/DataTypes/Config.hpp"

class    MimeTypes {

    static std::map<string, string>    Mime_Types;
    static Config*                     mimeTypesConfig;
    static std::string                 defaultMimeType;

    MimeTypes();


public:
    static std::map<string, string>     initDefaultMimeTypes();
    static void			                setMimeTypes(Config* config);
    static void			                setDefaultMimeType(const std::string& MimeType);
    static string    	                getDefaultMimeType();
    static string    	                getMimeType(const std::string& extention);
    static bool        	                hasMimeType(const std::string& extention);
};


#endif
