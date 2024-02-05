/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RedirectRoute.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysalmi <ysalmi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 17:10:43 by oelbouha          #+#    #+#             */
/*   Updated: 2024/02/05 12:42:43 by ysalmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RedirectRoute.hpp"

RedirectRoute::RedirectRoute(Route & route, ErrorPage& pages):  error_pages(pages), route(route) {
	const Config& config  = route.getConfig();
	
	std::vector<Config*> redirectBlock = config.getBlockConfigIfExist("redirect");
	std::vector<Config*>::iterator it = redirectBlock.begin();

	while (it != redirectBlock.end()) {
		Config* con = *it;
		code = std::stod(con->getInlineConfigIfExist("code"), NULL);
		location = con->getInlineConfigIfExist("location");
		++it;
	}
}

RedirectRoute::~RedirectRoute(){}

unsigned int RedirectRoute::getStatusCode() const  { return statusCode; }

const string&		RedirectRoute::getRoot() const { return route.getRoot(); }

const ErrorPage& 	RedirectRoute::getErrorPage() const { return error_pages; }

IResponse*	RedirectRoute::handle(IRequest& request)
{
	if (route.IsMethodAllowed(request.getMethod()) == false)
	{
		std::cout << "methods Not allowed " << std::endl;
		statusCode = 405;
		return error_pages.build(request, statusCode);
	}

	IResponse * response = new Response(request.getSocket());
	std::cout << "Sending response ... " << code << std::endl;;
	response->setHeader("location", location)
		.setStatusCode(code)
		.build();
	return response;
}


