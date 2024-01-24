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

IResponse*		Helper::BuildResponse(const IRequest& request, const IHandler& handler)
{
	unsigned int stsCode = handler.getStatusCode();
	IResponse * response = new Response(request.getSocket());
	ErrorPage &error_pages = handler.getErrorPage();

	if (error_pages.HasErrorPageFor(stsCode))
	{
		const string& filePath = error_pages.getErrorPagePath(stsCode);	
		response->setStatusCode(stsCode)
			.setHeader("connection", request.getHeader("Connection"))
			.setBodyFile(handler.getRoot() + filePath)
			.build();
		return response;
	}
	const string& filePath = error_pages.getDefaultErrorPage(stsCode);
	string root = handler.getRoot();
	int pos = root.rfind('/');
	root.erase(pos + 1, root.length());

	std::cout << "root :" << root + filePath << ":" << std::endl;
	
	response->setStatusCode(stsCode)
		.setHeader("connection", request.getHeader("Connection"))
		.setBodyFile(root + filePath)
		.build();
	return response;
}
