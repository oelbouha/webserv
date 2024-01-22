/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RedirectRoute.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelbouha <oelbouha@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 17:10:43 by oelbouha          #+#    #+#             */
/*   Updated: 2024/01/22 23:27:13 by oelbouha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RedirectRoute.hpp"

RedirectRoute::RedirectRoute(Config * config)
{
	code = std::stod(config->getInlineConfigIfExist("code"), NULL);
	URI = config->getInlineConfigIfExist("uri");
	allowedMethods = config->getListConfigIfExist("allowed_methods");
}

RedirectRoute::~RedirectRoute(){}

const string	RedirectRoute::setMethod(method_t m){
	switch (m){
	case GET:
		return "GET";
	case POST:
		return "POST";
	case DELETE:
		return "DELETE";
	case HEAD:
		return "HEAD";
	}
}

bool	RedirectRoute::IsMethodAllowed(method_t m){
	const string & method = setMethod(m);
	for(size_t i = 0; i < allowedMethods.size(); ++i){
		if (method == allowedMethods[i])
			return true;
	}
	return false;
}

IResponse*	RedirectRoute::handle(const IRequest& request){
	if (IsMethodAllowed(request.getMethod()) == false)
	{
		// return GenerateErrorPageResponse(request, 405);
	}
	IResponse * response = new Response(request.getSocket());
	response->setHeader("connection", request.getHeader("Connection"))
		.setHeader("Location", URI)
		.setHeader("content-type", "text/html")
		.setStatusCode(code)
		.build();
	return response;
}


