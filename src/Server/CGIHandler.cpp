/*	                     __          _
 *	   __  ___________ _/ /___ ___  (_)
 *	  / / / / ___/ __ `/ / __ `__ \/ /
 *	 / /_/ (__  ) /_/ / / / / / / / / 
 *	 \__, /____/\__,_/_/_/ /_/ /_/_/ 
 *	/____/	User: Youssef Salmi
 *			File: CGIHandler.cpp 
 */

#include "CGIHandler.hpp"
#include <string>
#include <unistd.h>
#include <vector>

CGIHandler::CGIHandler()
{}

CGIHandler::CGIHandler( const CGIHandler& c )
{(void)c;}

CGIHandler::~CGIHandler()
{}

CGIHandler&	CGIHandler::operator=( const CGIHandler& c )
{
    (void)c;
	return (*this);
}

ProxiedResponse*    CGIHandler::handle(IRequest* request)
{
    int fds[2];
    int err = pipe(fds);
    if (err)
        return NULL;

    int pid = fork();
    if (pid < 0)
        return NULL;
    
    if (pid == 0)
    {
        // cd to path
        err = chdir("/Users/ysalmi/code/in_progress/webserv/pages");
        if (err)
            exit(1);

        //build args;
        const std::string cgi = "/goinfre/ysalmi/brew/bin/python3";
        std::vector<const char *>    args;

        args.push_back("python3");
        args.push_back("index.py");

        // build env;
        std::vector<std::string>    e;

        e.push_back("URI=" + request->getURI());
        e.push_back("QUERY_STRING=" + request->getQuery());

        std::vector<const char*>  env;
        for (std::vector<std::string>::iterator it = e.begin(); it != e.end(); ++it)
            env.push_back(it->data());

        // excute script;
        const std::vector<const char*>&   execv_args = args, execv_env = env;
        execve(cgi.data(), execv_args.data(), execv_env.data());
        exit(1);
    }
    
}
