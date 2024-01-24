/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ErrorPage.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelbouha <oelbouha@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 22:36:00 by oelbouha          #+#    #+#             */
/*   Updated: 2024/01/24 12:22:54 by oelbouha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ERRORPAGE_HPP
#define ERRORPAGE_HPP

#include <iostream>

#include "src/Response/Response.hpp"
#include "src/Request/Request.hpp"
#include "src/DataTypes/Config.hpp"
#include "Utils.hpp"

class    ErrorPage {
    std::map<string, string>            error_pages;
    std::map<unsigned int, string>      DefaultErrorPages;

public:
    ErrorPage();
    
    std::map<unsigned int, string>	   initDefaultErrorPages();
    
    std::string   	getErrorPagePath(unsigned int stsCode);
    std::string   	getDefaultErrorPage(unsigned int stsCode);
    void            setErrorPage(Config& config);
    bool            HasErrorPageFor(unsigned int stscode);
    bool	        hasErrorCode(std::string code);
};

#endif