/*	                     __          _
 *	   __  ___________ _/ /___ ___  (_)
 *	  / / / / ___/ __ `/ / __ `__ \/ /
 *	 / /_/ (__  ) /_/ / / / / / / / / 
 *	 \__, /____/\__,_/_/_/ /_/ /_/_/ 
 *	/____/	User: Youssef Salmi
 *			File: ChunkedReader.cpp 
 */

#include "ChunkedReader.hpp"
#define CRLF  "\r\n"

ChunkedReader::ChunkedReader(IClientSocket& sock):
    mSocket(sock),
    mBuffer(CRLF),
    mContentLength(0),
    mCurrentChunkSize(0),
    mCurrentChunkRead(0),
    mEof(false),
    mTrailer(false)
{}

size_t  ChunkedReader::getContentLength() const
{
    return (mContentLength);
}

/**/

std::string ChunkedReader::read()
{
    if (mEof)
        return "";
    
    std::string ret;

    mBuffer += mSocket.readAll();

    if (mTrailer)
    {
        size_t  pos = mBuffer.find("\r\n\r\n");
        if (pos != std::string::npos)
        {
            mEof = true;
            // mSocket.retake(buffer.substr(pos + 4))
        }
        return ("");
    }

    while (!mBuffer.empty())
    {
    // there is an unfinished chunk
        if (mCurrentChunkSize - mCurrentChunkRead)
        {
            if (mBuffer.length() <= mCurrentChunkSize - mCurrentChunkRead)
            {
                ret += mBuffer;
                mBuffer.clear();
                mCurrentChunkRead += mBuffer.length();
                break;
            }
            ret += mBuffer.substr(0, mCurrentChunkSize - mCurrentChunkRead);
            mBuffer.erase(0, mCurrentChunkSize - mCurrentChunkRead);
            mCurrentChunkSize = 0;
            mCurrentChunkRead = 0;

            if (mBuffer == CRLF)
                break;
        }

    // no unfinished chunk
        // CRLF from previous chunk
        if (mBuffer[0] != '\r' || mBuffer[1] != '\n')
            throw RequestException("Bad Request 1", RequestException::BAD_REQUEST);
        mBuffer.erase(0, 2);

        if ( ! parseChunkHeader(mBuffer) )
            return (ret);

        if (mCurrentChunkSize == (size_t)-1)
            throw RequestException("Bad Request 2", RequestException::BAD_REQUEST);
        else if (mCurrentChunkSize == 0)
        {
            if (mBuffer.find(CRLF) == (size_t)0)
            {
                mEof = true;
                // mSocket.retake(mBuffer.substr(2))
                return (ret);
            }
            else {
                size_t  pos = mBuffer.find("\r\n\r\n");
                if (pos != std::string::npos)
                    mEof = true;
                    // mSocket.retake(mBuffer.substr(pos + 4))
                else
                    mTrailer = true;
                return (ret);
            }
        }
    }
    return (ret);
}


bool    ChunkedReader::parseChunkHeader(std::string& buffer)
{
    size_t  pos = buffer.find(CRLF);

    if (pos == std::string::npos)
    {
        // mBuffer = buffer;
        return (false);
    }

    std::string line = buffer.substr(0, pos);
    buffer.erase(0, pos + 2);

    // TODO: check for extentions
    mCurrentChunkRead = 0;

    pos = line.find(" ");
    if (pos == std::string::npos)
        mCurrentChunkSize = utils::hex_to_uint(line);
    else
        mCurrentChunkSize = utils::hex_to_uint(line.substr(pos));


    return (true);
}

bool	ChunkedReader::eof() const
{
    return (mEof);
}