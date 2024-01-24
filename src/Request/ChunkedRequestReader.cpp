/*	                     __          _
 *	   __  ___________ _/ /___ ___  (_)
 *	  / / / / ___/ __ `/ / __ `__ \/ /
 *	 / /_/ (__  ) /_/ / / / / / / / / 
 *	 \__, /____/\__,_/_/_/ /_/ /_/_/ 
 *	/____/	User: Youssef Salmi
 *			File: ChunkedRequestReader.cpp 
 */

#include "ChunkedRequestReader.hpp"
#define CRLF  "\r\n"

ChunkedRequestReader::ChunkedRequestReader(IClientSocket& sock):
    mSocket(sock),
    mContentLength(0),
    mCurrentChunkSize(0),
    mCurrentChunkRead(0),
    mEof(false),
    mTrailer(false)
{}

size_t  ChunkedRequestReader::getContentLength() const
{
    return (mContentLength);
}

std::string ChunkedRequestReader::read()
{
    if (mEof)
        return ("");

    std::string buffer = mSocket.readAll();

    if ( ! mBuffer.empty() )
    {
        buffer = mBuffer + buffer;
        mBuffer.clear();// mBuffer = ""
    }

    if (mTrailer)
    {
        size_t  pos = buffer.find("\r\n\r\n");
        if (pos != std::string::npos)
        {
            mEof = true;
            // mSocket.retake(buffer.substr(pos + 4))
        }
        return ("");
    }

    return (unchunk(buffer));
}

bool    ChunkedRequestReader::parseChunkHeader(std::string& buffer)
{
    size_t  pos = buffer.find(CRLF);

    if (pos == std::string::npos)
    {
        mBuffer = buffer;
        return (false);
    }

    std::string line = buffer.substr(0, pos);
    buffer.erase(0, pos + 2);

    // TODO: check for extentions

    pos = line.find(" ");
    if (pos == std::string::npos)
        mCurrentChunkSize = hexToInt(line);
    else
        mCurrentChunkSize = hexToInt(line.substr(pos));

    mCurrentChunkRead = 0;

    return (true);
}

std::string ChunkedRequestReader::unchunk(std::string& buffer)
{
    std::string ret;

    if (buffer.length() < mCurrentChunkSize - mCurrentChunkRead)
    {
        mCurrentChunkRead += buffer.length();
        mContentLength += buffer.length();
        return (buffer);
    }

    ret = buffer.substr(0, mCurrentChunkSize - mCurrentChunkRead);
    mContentLength += mCurrentChunkSize - mCurrentChunkRead;
    buffer.erase(0, mCurrentChunkSize - mCurrentChunkRead);

    if (mCurrentChunkSize)
    {
        if (buffer.find(CRLF) != 0)
            throw std::invalid_argument("request format error");
        buffer.erase(0, 2);
    }

    /*
     *  new chunk
     */
    if ( ! parseChunkHeader(buffer) )
        return (ret);

    if (mCurrentChunkSize == (size_t)-1)
        throw std::invalid_argument("request format error");
    else if (mCurrentChunkSize == 0)
    {
        if (buffer.find(CRLF) == (size_t)0)
        {
            mEof = true;
            // mSocket.retake(buffer.substr(2))
            return (ret);
        }
        else {
            size_t  pos = buffer.find("\r\n\r\n");
            if (pos != std::string::npos)
            {
                mEof = true;
                // mSocket.retake(buffer.substr(pos + 4))
                return (ret);
            }
            else
                mTrailer = true;
        }
    }

    return (ret + unchunk(buffer));
}



bool	ChunkedRequestReader::eof() const
{
    return (mEof);
}


size_t  ChunkedRequestReader::hexToInt(const std::string& str) const
{
    size_t              x;   
    std::stringstream   ss;

    ss << std::hex << str;
    ss >> x;
    return (x);
}
