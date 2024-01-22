/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RedirectRoute.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelbouha <oelbouha@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 17:11:11 by oelbouha          #+#    #+#             */
/*   Updated: 2024/01/22 21:18:53 by oelbouha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REDIRECTROUTE_HPP
#define REDIRECTROUTE_HPP

#include <iostream>

#include "src/Response/Response.hpp"
#include "src/Request/Request.hpp"
#include "src/DataTypes/Config.hpp"
#include "src/Interfaces/IHandler.hpp"
#include "Utils.hpp"

class RedirectRoute : public IHandler {
	std::vector<string> 	allowedMethods;
public:
	RedirectRoute(Config * config);
	// RedirectRoute( const RedirectRoute& s );
	~RedirectRoute();
	
	virtual IResponse*  handle(const IRequest&);
	
	bool			IsMethodAllowed(method_t m);
	const string	setMethod(method_t m);

private:
	unsigned int code;
	string	URI;
	string	location;
};

#endif