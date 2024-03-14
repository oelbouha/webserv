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
#include <iomanip>
#include <string>
#include <sstream>

#include <unistd.h>

/*
Logger::info.log(x).log(y).log(z).end();
*/


class Logger {
public:
    enum Level {
        DEBUG,
        INFO,
        WARNING,
        ERROR,
        FATAL
    };
    class File;

    static  Level   level;
    static  File    debug;
    static  File    info;
    static  File    warn;
    static  File    error;
    static  File    fatal;




    class   File {
        Level               msg_level;
        int                 fd;
        std::ostringstream  ss;
        std::string         slvl;

        File& operator=(const File& file);
        File(const File& file);
    public:
        File(Level lvl, int fd);
        ~File();

        template<typename T>
        File&   operator()(const T& msg) {
            if (level > msg_level) return (*this);

            ss << msg;
            return (*this);
        }

        File&   r();
        File&   l();

        template<typename T>
        File&   w(uint n, const T& msg) {
            if (level > msg_level) return (*this);

            ss << std::setw(n) << msg;
            return (*this);
        }

        void    flush();
    };
};

#endif