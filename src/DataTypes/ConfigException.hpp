/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigException.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelbouha <oelbouha@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/28 15:18:09 by oelbouha          #+#    #+#             */
/*   Updated: 2024/01/28 15:59:33 by oelbouha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#ifndef CONFIGEXCEPTION_HPP
#define CONFIGEXCEPTION_HPP

#include <iostream>
#include <string>
#include <cstring>

class ConfigException : public std::exception
{
	std::string message;

	public:
		ConfigException();
		ConfigException(const std::string& msg, const std::string& derictive, const std::string& value);
		~ConfigException() throw ();
		const char*	what( void ) const throw();

};
#endif