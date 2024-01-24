/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Helper.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelbouha <oelbouha@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 21:59:44 by oelbouha          #+#    #+#             */
/*   Updated: 2024/01/24 11:12:47 by oelbouha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HELPER_HPP
#define HELPER_HPP

#include <iostream>

#include "src/Response/Response.hpp"
#include "src/Server/ErrorPage.hpp"
#include "src/Interfaces/IHandler.hpp"
#include "src/Request/Request.hpp"
#include "src/DataTypes/Config.hpp"

class Helper{
	Helper();
	public:
		static IResponse*	BuildResponse(const IRequest& , const IHandler& handler);
};

#endif
