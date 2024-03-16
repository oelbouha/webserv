/*	                     __          _
 *	   __  ___________ _/ /___ ___  (_)
 *	  / / / / ___/ __ `/ / __ `__ \/ /
 *	 / /_/ (__  ) /_/ / / / / / / / /
 *	 \__, /____/\__,_/_/_/ /_/ /_/_/
 *	/____/	User: Youssef Salmi
 *			File: FileResponse.cpp
 */

#include "FileResponse.hpp"

int	FileResponse::bufferSize = 250000;

FileResponse::FileResponse(const IClientSocket &aSocket) :
	AResponse(aSocket),
	mFile(-1),
	mCursor(0),
	mEOF(false)
{}

FileResponse::FileResponse(const FileResponse &aFileResponse) :
	AResponse(aFileResponse.mSocket)
{}

FileResponse::~FileResponse() {
	::close(mFile);
}

FileResponse &FileResponse::operator=(const FileResponse &aFileResponse)
{
	if (this != &aFileResponse)
	{
	}
	return (*this);
}

FileResponse &FileResponse::setBody(const std::string &aFileName)
{
	mFile = ::open(aFileName.c_str(), O_RDONLY);
	if (mFile < 0) {
		if (::access(aFileName.c_str(), F_OK) != 0)
			throw ResponseException(aFileName);
		throw ResponseException(aFileName, ResponseException::PERMISSION_DENIED);
	}

	std::ifstream file(aFileName.data(), std::ifstream::ate | std::ifstream::binary);
	std::string contentLength = utils::to_string(file.tellg());
	file.close();
	setHeader("content-length", contentLength);

	const std::string &extension = utils::getExtension(aFileName);
	setHeader("content-type", MimeTypes::getMimeType(extension));

	return *this;
}

FileResponse &FileResponse::build()
{
	mRawResponse = "HTTP/1.1 " + AResponse::StatusCodes.at(mStatusCode) + "\r\n";

	for (string_string_map::iterator it = mHeaders.begin(); it != mHeaders.end(); ++it)
		mRawResponse += it->first + ": " + it->second + "\r\n";

	mRawResponse += "\r\n";

	return (*this);
}

void FileResponse::send()
{
	try
	{
		int r = 0;
		if (mRawResponse.length() < static_cast<size_t>(FileResponse::bufferSize))
		{
			char readBuffer[FileResponse::bufferSize];

			r = ::read(mFile, readBuffer, FileResponse::bufferSize);
			if (r < FileResponse::bufferSize)
				mEOF = true;

			mRawResponse += std::string(readBuffer, r);
		}

		mCursor = mSocket.write(mRawResponse);
		mRawResponse.erase(0, mCursor);
	}
	catch (const SocketException &e)
	{
		mEOF = true;
		mRawResponse.clear();
	}
}

bool FileResponse::done() const
{
	return (mEOF && mRawResponse.empty());
}