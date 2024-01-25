/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ErrorPage.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelbouha <oelbouha@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 22:39:26 by oelbouha          #+#    #+#             */
/*   Updated: 2024/01/25 22:50:12 by oelbouha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "ErrorPage.hpp"

ErrorPage::ErrorPage(){}
	
bool	ErrorPage::HasErrorPageFor(unsigned int stscode) {
	try{
		error_pages.at(std::to_string(stscode));
		return true;
	}
	catch(...){
		return false;
	}
}

bool	ErrorPage::hasErrorCode(std::string code) {
	try{
		error_pages.at(code);
		return true;
	}
	catch(...){
		return false;
	}
}

void	ErrorPage::setErrorPage(Config & config)
{
	std::vector<Config *> error_page = config.getBlockConfigIfExist("error_page");

	if (error_page.empty())
		return ;

	std::vector<Config *>::iterator it = error_page.begin();
	while (it != error_page.end()) {
		Config* error_pageConfig = *it;
		if (error_pageConfig->hasInline("file") && error_pageConfig->hasList("codes")) {
			string  file = error_pageConfig->getInlineConfig("file");
			
			std::vector<string> codes = error_pageConfig->getListConfig("codes");
			std::vector<string>::iterator cur = codes.begin();
			
			while (cur != codes.end()) {
				if (hasErrorCode(*cur) == false)
					error_pages[*cur] = file;
				++cur;
			}
		}
		++it;
	}
}

std::string   	ErrorPage::getErrorPagePath(unsigned int stsCode) {
	std::string code = std::to_string(stsCode);
	return error_pages.at(code);
}
