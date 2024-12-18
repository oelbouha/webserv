/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ErrorPages.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysalmi <ysalmi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 22:36:00 by oelbouha          #+#    #+#             */
/*   Updated: 2024/03/16 08:05:36 by ysalmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ErrorPages_HPP
#define ErrorPages_HPP


#include <iostream>

#include "src/Interfaces/IResponse.hpp"
#include "src/Response/FileResponse.hpp"
#include "src/Response/BufferResponse.hpp"
#include "src/Request/Request.hpp"
#include "src/DataTypes/Config.hpp"

#include "Utils.hpp"
#include "Types.hpp"

class    ErrorPages
{
    string_string_map    error_pages;

public:
    ErrorPages();

    void            setErrorPages(const std::vector<Config *>& error_page, const std::string& root);
    IResponse*      build(const IRequest& request, unsigned int status_code) const;
    void            dump();
};

#define CUSTOM_PAGE_BODY "\
<html>\
    <head>\
        <title>::title::</title>\
        <style>\
        body {\
            font-family: Arial, sans-serif;\
            background: #282828;\
            padding: 10px; \
            display: flex;\
            flex-direction: column;\
            justify-content: center;\
            align-items: center;\
            color: white;\
            width: 1080px;\
            max-width: 100%;\
            margin: 0 auto;\
            height: 100%;\
            min-height: 100vh;\
            box-sizing: border-box;\
        }\
        .error-code { font-size: 120px; color: #d9534f;}\
        .error-message { font-size: 24px; margin-top: 20px; }\
        </style>\
    </head>\
    <body>\
        <div class=\"error-code\"> ::code:: </div>\
        <div class=\"error-message\"> ::message:: </div>\
    </body>\
</html>\
"
#endif