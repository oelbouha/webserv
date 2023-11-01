#include "Logger.hpp"

Logger* Logger::mInstance = NULL;

Logger::Logger():
mLogLevel(logger::error),
mNextMsgLogLevel(logger::info),
mStream(std::cout)
{}

Logger::Logger( const logger::LogLevel& aLevel ):
mLogLevel(aLevel),
mNextMsgLogLevel(logger::info),
mStream(std::cout)
{}

Logger::~Logger(){}

ILogger&     Logger::getInstance( const logger::LogLevel& aLevel )
{
    if (Logger::mInstance == NULL)
        Logger::mInstance = new Logger(aLevel);
    return (*mInstance);
}

// std::ostream&   Logger::getStream(){
//     return (mStream);
// }

void        Logger::setLogLevel(const logger::LogLevel& aLevel){
    mLogLevel = aLevel;
}

Logger&     Logger::operator<<(const logger::LogLevel& aLevel){
    mNextMsgLogLevel = aLevel;
    // mStream << "changed to " << aLevel << std::endl;
    return *this;
}

Logger&     Logger::operator<<(const std::string& aMessage){
    mStream << aMessage;
    return *this;
}

Logger&     Logger::operator<<(const logger::endl_t& endl){
    (void)endl;
    mStream << std::endl;
    return *this;
}

Logger&     Logger::operator<<(const logger::flush_t& flush){
    (void)flush;
    mStream << std::flush;
    return *this;
}