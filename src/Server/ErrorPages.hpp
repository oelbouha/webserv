/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ErrorPages.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelbouha <oelbouha@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 22:36:00 by oelbouha          #+#    #+#             */
/*   Updated: 2024/01/22 23:52:22 by oelbouha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ERRORPAGES_HPP
#define ERRORPAGES_HPP

#include <iostream>

#include "src/Response/Response.hpp"
#include "src/Request/Request.hpp"
#include "src/DataTypes/Config.hpp"

class    ErrorPages {
    std::map<string, string>       error_pages;

public:
    ErrorPages();
    std::string   	getErrorPagePath(unsigned int stsCode);
    void            setErrorPage(Config* config, string root);
};

#endif