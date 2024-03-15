/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Upload.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysalmi <ysalmi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/19 18:46:19 by oelbouha          #+#    #+#             */
/*   Updated: 2024/03/15 06:58:34 by ysalmi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UPLOAD_HPP
#define UPLOAD_HPP

#include <iostream>
#include <ctime>

#include "src/Interfaces/IUpload.hpp"
#include "src/Interfaces/IClient.hpp"

#include "src/Request/Request.hpp"
#include "src/Request/BufferRequest.hpp"
#include "src/DataTypes/Config.hpp"


#include "Utils.hpp"


class Upload : public IUpload
{
	IRequest 		*request;
	bool			firstRead;
	bool			is_done;
	std::string 	upload_dir;
	std::string		body;
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
	
public:
	IClient*		client;

public:
	Upload(IRequest *request, const string& upload_path);
	~Upload();

	void 			handle();
	bool			done() const;
	Request*		getRequest();
	int				getSocketFd() const;


private:
	Upload&	operator=( const Upload& s );
	Upload( const Upload& s );
	
	void			createTmpFile();
	void			buildUploadRequest();
	bool			search(const string& buff, const string & line);
	string 			getFieldName(const std::string& name, const std::string& del);
};


#endif
