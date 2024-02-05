/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ErrorPage.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysalmi <ysalmi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 22:39:26 by oelbouha          #+#    #+#             */
/*   Updated: 2024/02/05 14:54:47 by ysalmi           ###   ########.fr       */
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
	IResponse*	response = new Response(request.getSocket());
	response->setStatusCode(status_code);
	try {
		std::string	error_page_path = error_pages.at(utils::to_string(status_code));
		response->setBodyFile(error_page_path)
			.build();
	} catch (const std::exception& e)
	{
		std::string	body = CUSTOM_PAGE_BODY;
		
		body.replace(body.find("::title::"), 9, Response::sStatusCodes.at(status_code));
		body.replace(body.find("::code::"), 8, utils::to_string(status_code));
		body.replace(body.find("::message::"), 11, Response::sStatusCodes.at(status_code));
		
		response->setBody(body)
			.build();
	}
	std::cout << "************** Built **************\n";
	std::cout << "          ***************" << std::endl;
	return (response);
}

void	ErrorPage::dump()
{
	std::map<string, string>::iterator	it = error_pages.begin();
	
	while (it != error_pages.end())
	{
		std::cout << it->first << " -> " << it->second << std::endl;
		++it;
	}
}