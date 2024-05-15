/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Upload.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysalmi <ysalmi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/19 18:46:19 by oelbouha          #+#    #+#             */
/*   Updated: 2024/05/15 14:17:16 by ysalmi           ###   ########.fr       */
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

#include "src/Server/ErrorPages.hpp"

#include "Logger.hpp"
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
	const ErrorPages*		error_pages;
	
public:
	IClient*		client;

public:
	Upload(IRequest *request, const string& upload_path);
	~Upload();

	int				getSocketFd() const;
	void			setErrorPages(const ErrorPages* error_pages);
	void 			handle();
	bool			done() const;
	Request*		getRequest();
	IResponse*		buildErrorPage(int code) const;


private:
	Upload&	operator=( const Upload& s );
	Upload( const Upload& s );
	
	void			createTmpFile();
	void			buildUploadRequest();
	std::string 	getFieldName(const std::string& name, const std::string& del);
	std::string 	getNextTmpName();
};


#endif
