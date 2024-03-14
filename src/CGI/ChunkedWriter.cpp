/*	                     __          _
 *	   __  ___________ _/ /___ ___  (_)
 *	  / / / / ___/ __ `/ / __ `__ \/ /
 *	 / /_/ (__  ) /_/ / / / / / / / / 
 *	 \__, /____/\__,_/_/_/ /_/ /_/_/ 
 *	/____/	User: Youssef Salmi
 *			File: ChunkedWriter.cpp 
 */

#include "ChunkedWriter.hpp"

ChunkedWriter::ChunkedWriter(const IClientSocket& socket) :
	mSocket(socket),
	mEOF(false)
{}


ChunkedWriter::~ChunkedWriter()
{}

void	ChunkedWriter::setHeader(const std::string& header)
{
	mHeader = header;
}

void	ChunkedWriter::append(const std::string& data)
{
	if (data.empty()) {
		mEOF = true;
		return;
	}
	mWaitingBuffer += data;
}

int	ChunkedWriter::write()
{
	int r = 0;
	if (mActiveBuffer.empty() == false) {
		r = mSocket.write(mActiveBuffer);
		mActiveBuffer.erase(0, r);
	}

	if (mActiveBuffer.empty()) {
		if (mWaitingBuffer.empty() && mHeader.empty()) return r;

		mActiveBuffer = mWaitingBuffer;
		mWaitingBuffer.clear();

		mActiveBuffer = utils::uint_to_hex(mActiveBuffer.length()) + "\r\n" + mActiveBuffer + "\r\n";

		if (mEOF)
			mActiveBuffer += "0\r\n\r\n";

		if (mHeader.empty() == false) {
			mActiveBuffer = mHeader + mActiveBuffer;
			mHeader.clear();
		}
		r += mSocket.write(mActiveBuffer);
	}
	return r;
}

bool	ChunkedWriter::done() const
{
	return mActiveBuffer.empty() && mWaitingBuffer.empty() && mEOF;
}
