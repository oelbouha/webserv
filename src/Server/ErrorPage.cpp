/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ErrorPage.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysalmi <ysalmi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 22:39:26 by oelbouha          #+#    #+#             */
/*   Updated: 2024/02/06 10:21:52 by ysalmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "ErrorPage.hpp"

ErrorPage::ErrorPage(){}
	
bool	ErrorPage::HasErrorPageFor(unsigned int stscode) const
{
	try{
		std::cout << "has error page for: " << utils::to_string(stscode) << std::endl;
		error_pages.at(utils::to_string(stscode));
		std::cout << "YES : " << error_pages.at(utils::to_string(stscode)) << std::endl;
		return true;
	}
	catch(...){
		return false;
	}
}

bool	ErrorPage::hasErrorCode(std::string code) const
{
	try{
		error_pages.at(code);
		return true;
	}
	catch(...){
		return false;
	}
}

void	ErrorPage::setErrorPage(const std::vector<Config *>& error_pages_config, const std::string& root)
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
				if (utils::isValidNumber(*cur) == false)
					throw ConfigException("error_page: Invalid Error Code number", "codes", *cur);
				error_pages[*cur] = file;
				++cur;
			}
		}
		++it;
	}
}

std::string   	ErrorPage::getErrorPagePath(unsigned int stsCode) const
{
	std::string code = utils::to_string(stsCode);
	return error_pages.at(code);
}

IResponse*      ErrorPage::build(const IRequest& request, unsigned int status_code) const
{
	try {
		IResponse*	response = new FileResponse(request.getSocket());
		response->setStatusCode(status_code);
		
		std::string	error_page_path = error_pages.at(utils::to_string(status_code));
		response->setBody(error_page_path)
			.build();
		
		return (response);
	} catch (const std::exception& e)
	{
		IResponse*	response = new BufferResponse(request.getSocket());
		response->setStatusCode(status_code);

		// Logger::Error( )
		
		std::string	body = CUSTOM_PAGE_BODY;
		
		utils::replace(body, "::title::", Response::StatusCodes.at(status_code));
		utils::replace(body, "::code::", utils::to_string(status_code));
		utils::replace(body, "::message::", Response::StatusCodes.at(status_code));
		
		response->setBody(body)
			.build();
			
		return (response);
	}
}

void	ErrorPage::dump()
{
	string_string_map::iterator	it = error_pages.begin();
	
	while (it != error_pages.end())
	{
		std::cout << it->first << " -> " << it->second << std::endl;
		++it;
	}
}