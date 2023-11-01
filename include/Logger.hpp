/*                       __          _
 *     __  ___________ _/ /___ ___  (_)
 *    / / / / ___/ __ `/ / __ `__ \/ /
 *   / /_/ (__  ) /_/ / / / / / / / / 
 *   \__, /____/\__,_/_/_/ /_/ /_/_/ 
 *  /____/  User: Youssef Salmi
 *          File: Logger.hpp 
 */

#pragma once
#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <iostream>
#include "ILogger.hpp"

class Logger: public ILogger
{
    static Logger*          mInstance;
    logger::LogLevel        mLogLevel;
    logger::LogLevel        mNextMsgLogLevel;
    std::ostream&           mStream;

    Logger();
    Logger( const logger::LogLevel& aLevel );
    public:

        static ILogger& getInstance( const logger::LogLevel& aLevel = logger::warning );
        void            setLogLevel( const logger::LogLevel& aLevel );

        Logger&         operator<<(const std::string& aMessage);
        Logger&         operator<<(const logger::LogLevel& aLevel);
        Logger&         operator<<(const logger::endl_t& endl);
        Logger&         operator<<(const logger::flush_t& flush);

        ~Logger();

};

#define INFO(x) Logger::getInstance() << logger::info << x
#endif