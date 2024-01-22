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

vector<Config*>	Helper::getBlockConfigIfExist(Config &config, const string& property){
	vector<Config*> ret;
	if (config.hasBlock(property) == true){
		ret = config.getBlockConfig(property);
	}
	return ret;
}

vector<string>	Helper::getListConfigIfExist(Config &config, const string& property){
	vector<string> ret;
	if (config.hasBlock(property) == true){
		ret = config.getListConfig(property);
	}
	return ret;
}

const string 	Helper::getInlineConfigIfExist(Config &config, const string& property){

	if (config.hasBlock(property) == true){
		return (config.getInlineConfig(property));
	}
	return "";
}