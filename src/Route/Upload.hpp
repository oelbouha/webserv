/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Upload.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelbouha <oelbouha@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/19 18:46:19 by oelbouha          #+#    #+#             */
/*   Updated: 2024/02/06 19:04:40 by oelbouha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UPLOAD_HPP
#define UPLOAD_HPP

#include <iostream>

#include "src/Interfaces/IUpload.hpp"

#include "src/Request/Request.hpp"
#include "src/DataTypes/Config.hpp"


#include "Utils.hpp"


class Upload : public IUpload {
	IRequest 		*request;
	std::string 	upload_dir;
	bool			firstRead;
	bool			is_done;
	std::string		body;

public:
	Upload(IRequest *request, const string& upload_path);
	~Upload();

	int				getSocketFd() const;

	std::string 	handle();
	IRequest*		getRequest();
	bool			done() const;

private:
	std::ofstream	file_stream;
	std::string		start;
	std::string		buff;
	std::string		name;
	std::string		boundry;
	std::string 	field_name;
	std::string 	file_name;
	std::string 	file_content_type;
	std::string 	file_path;

private:
	Upload&	operator=( const Upload& s );
	Upload( const Upload& s );
	
	bool			search(const string& buff, const string & line);
	void			SetupBoundry(const std::string& type);
	std::string 	getFieldName(std::string name, std::string end);
};


#endif