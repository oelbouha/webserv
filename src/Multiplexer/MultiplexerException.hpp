/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MultiplexerException.hpp                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelbouha <oelbouha@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/11 11:59:16 by oelbouha          #+#    #+#             */
/*   Updated: 2024/05/11 12:12:19 by oelbouha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef MULTIPLEXEREXCEPTION_HPP
#define MULTIPLEXEREXCEPTION_HPP

#include <iostream>
#include <map>
#include <queue>

#include <sys/select.h>
#include <sys/time.h>


class MultiplexerException : public std::exception {
	std::string message;
public:
	MultiplexerException();
  	MultiplexerException(const std::string& aMessage);
  	~MultiplexerException() throw();

  	const char *what() const throw();
};


#endif