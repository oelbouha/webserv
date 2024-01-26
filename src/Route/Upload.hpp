/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Upload.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelbouha <oelbouha@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/19 18:46:19 by oelbouha          #+#    #+#             */
/*   Updated: 2024/01/26 22:36:03 by oelbouha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UPLOAD_HPP
#define UPLOAD_HPP

#include <iostream>
#include "src/Response/Response.hpp"
#include "src/Response/ProxiedResponse.hpp"
#include "src/Request/Request.hpp"
#include "src/DataTypes/Config.hpp"
#include "src/Server/Server.hpp"
#include "Utils.hpp"


class Upload{
		bool		done;
		bool		firstRead;
	public:
		Upload(const IRequest& request);
		Upload&	operator=( const Upload& s );
		Upload( const Upload& s );
		~Upload();

		std::string handle(const IRequest& request);
		bool	search(const string& buff, const string & line);
		bool	IsDone() const;
		std::string getFieldName();
		std::string getFileName();
		std::string getContentType();

		std::string		buff;
	private:
    	std::ofstream	file_stream;
		std::string		body;
		std::string		start;
		std::string		name;
		std::string		boundry;
		std::string 	field_name;
    	std::string 	file_name;
    	std::string 	file_content_type;
    	std::string 	file_path;
    	std::string 	upload_dir;
};


#endif