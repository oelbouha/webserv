/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ErrorPages.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelbouha <oelbouha@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 22:39:26 by oelbouha          #+#    #+#             */
/*   Updated: 2024/01/22 23:50:06 by oelbouha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "ErrorPages.hpp"

ErrorPages::ErrorPages(){
	
}

void	ErrorPages::setErrorPage(Config * config, string root)
{
	std::vector<Config *> error_page = config->getBlockConfigIfExist("error_page");
	
	if (error_page.empty())
		return ;
	std::vector<Config *>::iterator it = error_page.begin();
	while (it != error_page.end())
	{
		Config* error_pageConfig = *it;
		if (error_pageConfig->hasInline("file") && error_pageConfig->hasList("codes"))
		{
			string  file = error_pageConfig->getInlineConfig("file");
			
			std::vector<string> codes = error_pageConfig->getListConfig("codes");
			std::vector<string>::iterator cur = codes.begin();
			
			while (cur != codes.end())
			{
				// check if it exist dont add it
				error_pages[*cur] = root + file;
				++cur;
			}
		}
		++it;
	}
}

std::string   	ErrorPages::getErrorPagePath(unsigned int stsCode){
	std::string code = std::to_string(stsCode);
	return error_pages.at(code);
}
