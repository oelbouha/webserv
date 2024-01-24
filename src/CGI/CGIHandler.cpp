/*	                     __          _
 *	   __  ___________ _/ /___ ___  (_)
 *	  / / / / ___/ __ `/ / __ `__ \/ /
 *	 / /_/ (__  ) /_/ / / / / / / / / 
 *	 \__, /____/\__,_/_/_/ /_/ /_/_/ 
 *	/____/	User: Youssef Salmi
 *			File: CGIHandler.cpp 
 */

#include "CGIHandler.hpp"
#include "DescriptorProxyRequest.hpp"
#include "ProxyPair.hpp"
#include "ProxyResponse.hpp"
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

static char**   vectorToCStringArray(const std::vector<std::string>& vec);

ProxyPair*    CGIHandler::handle(IRequest* request)
{
    // setting up pipes
    int input[2];
    int output[2];

    int err = pipe(input);
    if (err)
        return NULL;
    err = pipe(output);
    if (err)
        return NULL;

    // forking for cgi process
    int pid = fork();
    if (pid < 0)
        return NULL;
    
    // cgi porcess
    if (pid == 0)
    {
        close(input[1]);
        close(output[0]);

        dup2(input[0], 0);
        dup2(output[1], 1);

        close(input[0]);
        close(output[1]);

        // cd to path
        err = chdir("/Users/ysalmi/code/in_progress/webserv/pages");
        if (err)
            exit(1);

        //build args;
        const std::string cgi = "/goinfre/ysalmi/brew/bin/python3";

        std::vector<std::string>    args;
        args.push_back("python3");
        args.push_back("index.py");

        // build env;
        std::vector<std::string>    e;

        e.push_back("URI=" + request->getURI());
        e.push_back("QUERY_STRING=" + request->getQuery());

        // excute script;
        execve(cgi.data(), vectorToCStringArray(args), vectorToCStringArray(e));
        exit(1);
    }

    // main process - webserv
    //
    close(input[0]);
    close(output[1]);

    IProxyRequest*  req = new DescriptorProxyRequest(input[1], *request);
    IProxyResponse* res = new ProxyResponse(output[0], request->getSocket());
    ProxyPair*      ret = new ProxyPair(pid, req, res);
    
    return (ret);
}

static char**   vectorToCStringArray(const std::vector<std::string>& vec)
{
    char**  ret = new char*[vec.size() + 1];
    int     k = 0;
    for (std::vector<std::string>::const_iterator it = vec.begin(); it != vec.end(); ++it)
    {
        char*   str = new char[it->length() + 1];
        for (size_t i = 0; i < it->length(); ++i)
            str[i] = static_cast<char>(it->at(i));
        str[it->length()] = 0;
        ret[k++] = str;
    }
    ret[k] = NULL;
    return (ret);
}
