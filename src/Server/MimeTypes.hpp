/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MimeTypes.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysalmi <ysalmi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/23 17:51:14 by oelbouha          #+#    #+#             */
/*   Updated: 2024/02/07 10:45:25 by ysalmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#ifndef MIMETPE_HPP
#define MIMETPE_HPP

#include <iostream>

#include "src/DataTypes/Config.hpp"

class    MimeTypes {
    static Config*          mimeTypes;
    static std::string      defaultType;

    MimeTypes();
    static void          setupNeededTypes();

public:
    static void			 setMimeTypes(Config* config);
    static void			 setDefault(const std::string& default_mime_type);
    static const string& getMimeType(const std::string& extention);
};


#endif
