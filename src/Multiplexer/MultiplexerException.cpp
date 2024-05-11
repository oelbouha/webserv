/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MultiplexerException.cpp                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelbouha <oelbouha@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/11 12:04:25 by oelbouha          #+#    #+#             */
/*   Updated: 2024/05/11 12:22:26 by oelbouha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "MultiplexerException.hpp"


MultiplexerException::MultiplexerException(const std::string& aMessage) {
	message = aMessage;
}

MultiplexerException::~MultiplexerException() throw() {}

const char* MultiplexerException::what() const throw() {
	return message.c_str();
}

MultiplexerException::MultiplexerException() : message("Multiplexer Failed ") {}