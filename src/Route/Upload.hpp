/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Upload.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysalmi <ysalmi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/19 18:46:19 by oelbouha          #+#    #+#             */
/*   Updated: 2024/02/07 09:46:58 by ysalmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UPLOAD_HPP
#define UPLOAD_HPP

#include <iostream>
#include <ctime>

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

	IRequest*		getRequest();
	void 			handle();
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
	unsigned int	count;

private:
	Upload&	operator=( const Upload& s );
	Upload( const Upload& s );
	
	void			createTmpFile();
	void			buildUploadRequest();
	bool			search(const string& buff, const string & line);
	string 			getFieldName(const std::string& name, const std::string& del);
};


#endif