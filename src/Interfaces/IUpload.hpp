

#pragma once
#ifndef IUPLOAD_HPP
#define IUPLOAD_HPP

#include "src/Response/Response.hpp"
#include "src/Request/Request.hpp"
#include "src/DataTypes/Config.hpp"
#include "Utils.hpp"

class IUpload {
public:
	virtual	~IUpload() {};
	
	virtual int				getSocketFd() const = 0;
	virtual void 			handle() = 0;
	virtual IRequest*		getRequest() = 0;
	virtual bool			done() const = 0;
};

#endif