/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ErrorPages.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysalmi <ysalmi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 22:39:26 by oelbouha          #+#    #+#             */
/*   Updated: 2024/03/16 08:05:56 by ysalmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "ErrorPages.hpp"

ErrorPages::ErrorPages(){}


void	ErrorPages::setErrorPages(const std::vector<Config *>& error_pages_config, const std::string& root)
{
	if (error_pages_config.empty())
		return ;

	std::vector<Config *>::const_iterator it = error_pages_config.begin();
	while (it != error_pages_config.end())
	{
		Config* error_page = *it;
		if (error_page->hasInline("file") && error_page->hasList("codes"))
		{
			string  file = root + error_page->getInlineConfig("file");
			
			std::vector<string> codes = error_page->getListConfig("codes");
			std::vector<string>::iterator cur = codes.begin();
			
			while (cur != codes.end())
			{
				if (utils::is_valid_number(*cur) == false)
					throw ConfigException("error_page: Invalid Error Code number", "codes", *cur);
				error_pages[*cur] = file;
				++cur;
			}
		}
		++it;
	}
}

IResponse*      ErrorPages::build(const IRequest& request, unsigned int status_code) const
{
	try {
		const std::string& error_page_path = error_pages.at(utils::to_string(status_code));
		IResponse*	response = new FileResponse(request.getSocket());
		response->setStatusCode(status_code).setBody(error_page_path);
		
		return (response);
	}
	catch (const std::exception& e)
	{
		IResponse*	response = new BufferResponse(request.getSocket());
		
		std::string	body = CUSTOM_PAGE_BODY;
		utils::replace(body, "::title::", AResponse::StatusCodes.at(status_code));
		utils::replace(body, "::code::", utils::to_string(status_code));
		utils::replace(body, "::message::", AResponse::StatusCodes.at(status_code));
		
		response->setStatusCode(status_code).setBody(body);
		return (response);
	}
}

void	ErrorPages::dump()
{
	string_string_map::iterator	it = error_pages.begin();
	
	while (it != error_pages.end())
	{
		Logger::debug (it->first)(" -> ")(it->second).flush();
		++it;
	}
}