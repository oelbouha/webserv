/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RedirectRoute.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelbouha <oelbouha@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 17:10:43 by oelbouha          #+#    #+#             */
/*   Updated: 2024/01/26 13:01:38 by oelbouha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RedirectRoute.hpp"

RedirectRoute::RedirectRoute(Route & route, ErrorPage& pages):  error_pages(pages), route(route) {
	const Config& config  = route.getConfig();
	
	std::vector<Config*> redirectBlock = config.getBlockConfigIfExist("redirect");
	std::vector<Config*>::iterator it = redirectBlock.begin();

	while (it != redirectBlock.end()) {
		Config* config = *it;
		std::string code = config->getInlineConfigIfExist("code");
		if (utils::isValidNumber(code))
			code = std::stod(code, NULL);
		location = config->getInlineConfigIfExist("location");
		++it;
	}
}

RedirectRoute::~RedirectRoute(){}

unsigned int RedirectRoute::getStatusCode() const  { return statusCode; }

string		RedirectRoute::getRoot() const { return route.getRoot(); }

ErrorPage& 	RedirectRoute::getErrorPage() const { return error_pages; }

IResponse*	RedirectRoute::handle(const IRequest& request) {
	if (route.IsMethodAllowed(request.getMethod()) == false) {
		statusCode = 405;
		return Helper::BuildResponse(request, *this);
	}

	IResponse * response = new Response(request.getSocket());
	response->setHeader("connection", request.getHeader("Connection"))
		.setHeader("location", location)
		.setStatusCode(code)
		.build();
	return response;
}


