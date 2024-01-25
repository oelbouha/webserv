/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Helper.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelbouha <oelbouha@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 21:59:33 by oelbouha          #+#    #+#             */
/*   Updated: 2024/01/22 22:05:31 by oelbouha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Helper.hpp"

std::string		BuildCustumPage(unsigned int stsCode){
	std::string body = "<style>\
		    body {\
            font-family: Arial, sans-serif;\
            background-color: #f4f4f4;\
            color: #333;\
            text-align: center;\
            padding: 50px; }\
        .error-code { font-size: 120px; color: #d9534f; /* Bootstrap's danger color */}\
        .error-message { font-size: 24px; margin-top: 20px; }\
    </style>";
	body += "<body> <div class=\"error-code\">" + std::to_string(stsCode) + "</div>";
	body += "<div class=\"error-message\">" + Response::sStatusCodes.at(stsCode) + "</div>";
	body += "<p>Your browser sent a request that this server could not understand.</p> </body>";
	return body;
}

IResponse*		Helper::BuildResponse(const IRequest& request, const IHandler& handler)
{
	unsigned int stsCode = handler.getStatusCode();
	std::cout << "status code >" << stsCode << std::endl;
	IResponse * response = new Response(request.getSocket());
	ErrorPage &error_pages = handler.getErrorPage();

	if (error_pages.HasErrorPageFor(stsCode))
	{
		const string& filePath = error_pages.getErrorPagePath(stsCode);	
		// std::cout << "root :" << handler.getRoot() + filePath << ":" << std::endl;
		response->setStatusCode(stsCode)
			.setHeader("connection", request.getHeader("Connection"))
			.setBodyFile(handler.getRoot() + filePath)
			.build();
		return response;
	}
	response->setStatusCode(stsCode)
		.setHeader("connection", request.getHeader("Connection"))
		.setBody(BuildCustumPage(stsCode))
		.build();
	return response;
}
