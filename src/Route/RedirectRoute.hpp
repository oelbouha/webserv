/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RedirectRoute.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysalmi <ysalmi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 17:11:11 by oelbouha          #+#    #+#             */
/*   Updated: 2024/02/05 11:41:09 by ysalmi           ###   ########.fr       */
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
	
	virtual const ErrorPage& 	getErrorPage() const;
	virtual IResponse*  		handle(IRequest&);
	virtual const string&		getRoot() const;
	unsigned int    			getStatusCode() const;
	

private:
	ErrorPage		&error_pages;
	Route			&route;
	string			location;
	unsigned int 	code;
	unsigned int 	statusCode;
};

#endif