/*	                     __          _
 *	   __  ___________ _/ /___ ___  (_)
 *	  / / / / ___/ __ `/ / __ `__ \/ /
 *	 / /_/ (__  ) /_/ / / / / / / / / 
 *	 \__, /____/\__,_/_/_/ /_/ /_/_/ 
 *	/____/	User: Youssef Salmi
 *			File: FileResponse.hpp 
 */

#pragma once
#ifndef FILERESPONSE_HPP
#define FILERESPONSE_HPP

#include "src/Response/AResponse.hpp"

class FileResponse : public AResponse
{
	int			mFile;
	std::string	mRawResponse;
	size_t		mCursor;
	bool		isComplete;

	FileResponse(const FileResponse &aFileResponse);
	FileResponse&	operator=(const FileResponse &aFileResponse);
public:
	FileResponse(const IClientSocket &aSocket);
	~FileResponse();

	FileResponse&	setBody(const std::string &aBody);
	// FileResponse&	setBodyFile(const std::string &aFileName);
	FileResponse&	build();

	void send();
	bool done() const;

};

#endif
