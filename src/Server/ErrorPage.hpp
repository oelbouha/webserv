/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ErrorPage.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysalmi <ysalmi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 22:36:00 by oelbouha          #+#    #+#             */
/*   Updated: 2024/02/05 14:53:43 by ysalmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ERRORPAGE_HPP
#define ERRORPAGE_HPP

#define CUSTOM_PAGE_BODY "\
<html> \
    <head> \
        <title>::title::</title> \
        <style> \
            body { \
                font-family: Arial, sans-serif; \
                background-color: #f4f4f4; \
                color: #333; \
                text-align: center; \
                padding: 50px; \
            } \
        .error-code { font-size: 120px; color: #d9534f;} \
        .error-message { font-size: 24px; margin-top: 20px; } \
        </style> \
    </head> \
    <body> \
        <div class=\"error-code\"> ::code:: </div> \
        <div class=\"error-message\"> ::message:: </div> \
    </body>\
</html>\
"

#include <iostream>

#include <src/Interfaces/IResponse.hpp>
#include "src/Response/Response.hpp"
#include "src/Request/Request.hpp"
#include "src/DataTypes/Config.hpp"
#include "Utils.hpp"

class    ErrorPage
{
    // std::vector<string>         paths;
    std::map<string, string>    error_pages;

public:
    ErrorPage();

    std::string   	getErrorPagePath(unsigned int sttsCode) const;
    void            setErrorPage(const std::vector<Config *>& error_page, const std::string& root);
    bool            HasErrorPageFor(unsigned int stscode) const;
    bool	        hasErrorCode(std::string code) const;
    IResponse*      build(const IRequest& request, unsigned int status_code) const;
    void            dump();
};

#endif