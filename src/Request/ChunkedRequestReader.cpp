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
    mBuffer(CRLF),
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

/**/

std::string ChunkedRequestReader::read()
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
/**/

/*
std::string ChunkedRequestReader::read()
{
    if (mEof)
        return ("");

    std::string buffer = mBuffer + mSocket.readAll();

    if ( ! mBuffer.empty() && mBuffer != CRLF)
        if ( ! parseChunkHeader(buffer) )
            return ("");

    mBuffer.clear();

    std::cout << (int)buffer[0] << " - " << (int)buffer[1] << " ******* " << std::endl;

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
*/

bool    ChunkedRequestReader::parseChunkHeader(std::string& buffer)
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
        mCurrentChunkSize = hexToInt(line);
    else
        mCurrentChunkSize = hexToInt(line.substr(pos));


    return (true);
}

std::string ChunkedRequestReader::unchunk(std::string& buffer)
{
    std::string ret;

    std::cout << std::endl;
    std::cout << "chunk size: " << mCurrentChunkSize << std::endl;
    std::cout << "read  size: " << mCurrentChunkRead << std::endl;
    std::cout << "buffer len: " << buffer.length() << std::endl;
    std::cout << "start of buff: ";

    for (unsigned int i = 0; i < 10; i++)
    {
        if (std::isprint(buffer[i]))
            std::cout << buffer[i];
        else
            std::cout << " $" << (int)buffer[i] << " ";
    }
    std::cout << std::endl;
    std::cout << "end of buff: ";
    for (unsigned int i = 0; i < 10 && buffer.length() > i + mCurrentChunkSize - 1; i++)
    {
        if (std::isprint(buffer[i + mCurrentChunkSize - 1]))
            std::cout << buffer[i + mCurrentChunkSize - 1];
        else
            std::cout << " $" << (int)buffer[i + mCurrentChunkSize - 1] << " ";
    }
    std::cout << std::endl;

    if (buffer.length() <= mCurrentChunkSize - mCurrentChunkRead)
    {
        mCurrentChunkRead += buffer.length();
        mContentLength += buffer.length();
        return (buffer);
    }


    ret = buffer.substr(0, mCurrentChunkSize - mCurrentChunkRead);
    mContentLength += mCurrentChunkSize - mCurrentChunkRead;
    buffer.erase(0, mCurrentChunkSize - mCurrentChunkRead);
    mCurrentChunkRead += ret.length();

    std::cout << "*start of buff: ";
    for (unsigned int i = 0; i < 10; i++)
    {
        if (std::isprint(buffer[i]))
            std::cout << buffer[i];
        else
            std::cout << " $" << (int)buffer[i] << " ";
    }
    std::cout << std::endl;

    // if (mCurrentChunkSize != 0)
    // {
        if (buffer.find(CRLF) != 0){
            // std::cout << ret << std::endl;
            std::cout << mCurrentChunkSize << " - " << mCurrentChunkRead << std::endl;
            std::cout << (int)buffer[0] << " - " << (int)buffer[1] << std::endl;
            throw RequestException("Bad Request 1", RequestException::BAD_REQUEST);
        }
        if (buffer == CRLF)
            return (ret);
        buffer.erase(0, 2);

    // }

    /*
     *  new chunk
     */
    if ( ! parseChunkHeader(buffer) )
        return (ret);

    if (mCurrentChunkSize == (size_t)-1)
        throw RequestException("Bad Request 2", RequestException::BAD_REQUEST);
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
                mEof = true;
                // mSocket.retake(buffer.substr(pos + 4))
            else
                mTrailer = true;
            return (ret);
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
