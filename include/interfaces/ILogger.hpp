/*                       __          _
 *     __  ___________ _/ /___ ___  (_)
 *    / / / / ___/ __ `/ / __ `__ \/ /
 *   / /_/ (__  ) /_/ / / / / / / / / 
 *   \__, /____/\__,_/_/_/ /_/ /_/_/ 
 *  /____/  User: Youssef Salmi
 *          File: ILogger.hpp 
 */

#pragma once
#ifndef ILOGGER_HPP
#define ILOGGER_HPP

#include <iostream>
#include <ostream>
#include <string>
#include <sstream>


namespace logger
{
    class endl_t{};
    class flush_t{};

    const endl_t    endl;
    const flush_t   flush;

    extern std::ostream&   inform;

    template <typename T>
    void    info_(const T& aMsg)
    {
        std::cout << aMsg;
    }

    enum LogLevel
    {
        debug,
        info,
        warning,
        error
    };
}

class ILogger
{
    public:
        virtual     ~ILogger(){};

        virtual ILogger&    operator<<(const logger::LogLevel& aLevel) = 0;
        virtual ILogger&    operator<<(const logger::endl_t& endl) = 0;
        virtual ILogger&    operator<<(const logger::flush_t& flush) = 0;
        virtual ILogger&    operator<<(const std::string& aMessage) = 0;


        template <typename T>
        ILogger&    operator<<(const T& aMessage){
            std::stringstream   s; s << aMessage;
            return *this << s.str();
        };

};

/*

Logger::info("klsdfksdf %s %d\n", var, a);

logger << LogLevel::info << "message" << a << std::endl;

*/
#endif