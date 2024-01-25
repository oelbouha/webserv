/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RedirectRoute.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelbouha <oelbouha@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 17:11:11 by oelbouha          #+#    #+#             */
/*   Updated: 2024/01/24 19:35:32 by oelbouha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REDIRECTROUTE_HPP
#define REDIRECTROUTE_HPP

#include <iostream>

#include "src/Response/Response.hpp"
#include "src/Request/Request.hpp"
#include "src/Route/Route.hpp"
#include "src/DataTypes/Config.hpp"
#include "src/Interfaces/IHandler.hpp"
#include "Utils.hpp"

class Route;

class RedirectRoute : public IHandler {
public:
	RedirectRoute(Route & route, ErrorPage& pages);
	~RedirectRoute();
	
	virtual ErrorPage& 		getErrorPage() const;
	virtual IResponse*  	handle(const IRequest&);
	virtual string			getRoot() const;
	unsigned int    		getStatusCode() const;
	

private:
	ErrorPage		&error_pages;
	Route			&route;
	string			location;
	unsigned int 	code;
	unsigned int 	statusCode;
};

#endif