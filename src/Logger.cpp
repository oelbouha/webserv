/*	                     __          _
 *	   __  ___________ _/ /___ ___  (_)
 *	  / / / / ___/ __ `/ / __ `__ \/ /
 *	 / /_/ (__  ) /_/ / / / / / / / / 
 *	 \__, /____/\__,_/_/_/ /_/ /_/_/ 
 *	/____/	User: Youssef Salmi
 *			File: CGIResponse.cpp 
 */

#include "Logger.hpp"

Logger::Level   Logger::level = Logger::INFO;
Logger::File    Logger::debug(Logger::DEBUG, 1);
Logger::File    Logger::info(Logger::INFO, 1);
Logger::File    Logger::warn(Logger::WARNING, 2);
Logger::File    Logger::error(Logger::ERROR, 2);
Logger::File    Logger::fatal(Logger::FATAL, 2);

Logger::File::File(Logger::Level lvl, int fd) :
    msg_level(lvl),
    fd(fd)
{
    switch (msg_level)
    {
    case DEBUG:
        slvl = "DEBUG";
        break;
    case INFO:
        slvl = "INFO";
        break;
    case WARNING:
        slvl = "WARNING";
        break;
    case ERROR:
        slvl = "ERROR";
        break;
    case FATAL:
        slvl = "FATAL";
        break;
    default:
        break;
    }
    ss << std::left << std::setw(10) << slvl << ": ";
}

Logger::File::File(const File& file) :
    msg_level(file.msg_level),
    fd(file.fd)
{}

Logger::File::~File()
{
    ::close(fd);
}

Logger::File&   Logger::File::operator=(const File& file)
{
    if (this != &file) {
        msg_level = file.msg_level;
        fd = file.fd;
    }
    return (*this);
}

Logger::File&   Logger::File::r() {
    ss << std::right;
    return (*this);
}
Logger::File&   Logger::File::l() {
    ss << std::left;
    return (*this);
}

void    Logger::File::flush() {
    if (level > msg_level) return;

    ss << std::endl;
    const std::string&    buffer = ss.str();
    ::write(fd, buffer.data(), buffer.length());
    ss.str("");
    ss << std::left << std::setw(10) << slvl << ": ";
}