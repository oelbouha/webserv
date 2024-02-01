/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MimeTypes.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelbouha <oelbouha@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/23 17:51:14 by oelbouha          #+#    #+#             */
/*   Updated: 2024/01/24 13:10:58 by oelbouha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MIMETPE_HPP
#define MIMETPE_HPP

#include <iostream>
#include "src/DataTypes/Config.hpp"

class    MimeTypes {
    static Config*          mimeTypesConfig;
    static std::string      defaultMimeType;

    MimeTypes();


public:
    static void			 setMimeTypes(Config* config);
    static string    	 getMimeType(const std::string& extention);
};


#endif
