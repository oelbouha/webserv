/*	                     __          _
 *	   __  ___________ _/ /___ ___  (_)
 *	  / / / / ___/ __ `/ / __ `__ \/ /
 *	 / /_/ (__  ) /_/ / / / / / / / / 
 *	 \__, /____/\__,_/_/_/ /_/ /_/_/ 
 *	/____/	User: Youssef Salmi
 *			File: CGIHandler.cpp 
 */

#include "CGIHandler.hpp"

CGIHandler::CGIHandler()
{}

ProxyPair    CGIHandler::handle(IRequest* request, const std::string& path)
{
    std::string file = path;
    // setting up pipes
    int input[2];
    int output[2];

    int err = pipe(input);
    if (err)
        return ProxyPair();
    err = pipe(output);
    if (err)
        return ProxyPair();

    // forking for cgi process
    int pid = fork();
    if (pid < 0)
        return ProxyPair();
    
    // cgi porcess
    if (pid == 0)
    {
        std::vector<std::string> args, env = compileEnv(*request);

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
        args.push_back(file);
        // excute script;
        execve(file.c_str(), utils::vector_to_cstring_array(args), utils::vector_to_cstring_array(env));
        perror("cgi");
        exit(1);
    }

    // main process - webserv
    //
    close(input[0]);
    close(output[1]);

    IProxyRequest*  req = new CGIRequest(input[1], *request);
    IProxyResponse* res = new CGIResponse(output[0], request->getSocket());
    ProxyPair      ret(pid, req, res);
    
    return (ret);
}

std::vector<std::string>    CGIHandler::compileEnv(IRequest& req)
{
    std::vector<std::string> env;
    // GATEWAY_INTERFACE
    env.push_back("GATEWAY_INTERFACE=CGI/1.1");
    // SERVER_SOFTWARE
    env.push_back("SERVER_SOFTWARE=webserv/1.0");
    // SERVER_PROTOCOL
    env.push_back("SERVER_PROTOCOL=HTTP/1.1");
    // SERVER_NAME
    env.push_back("SERVER_NAME=" + req.getHeader("host"));
    // SERVER_PORT
    env.push_back("SERVER_PORT=" + utils::to_string(req.getIncomingPort()));
    // REQUEST_METHOD
    env.push_back("REQUEST_METHOD=" + req.getMethod());
    // REQUEST_URI
    env.push_back("REQUEST_URI=" + req.getURI());
    // PATH_INFO
    env.push_back("PATH_INFO=" + req.getURI());
    // PATH_TRANSLATED
    // env.push_back("PATH_TRANSLATED=/Users/ysalmi/code/in_progress/webserv/pages/tester.py");
    // SCRIPT_NAME
    env.push_back("SCRIPT_NAME=" + req.getURI());
    // DOCUMENT_ROOT
    // env.push_back("DOCUMENT_ROOT=/Users/ysalmi/code/in_progress/webserv/pages");
    // QUERY_STRING
    if (req.getQuery().empty() == false)
        env.push_back("QUERY_STRING=" + req.getQuery());

    // CONTENT_TYPE
    const std::string& content_type = req.getHeader("content-type");
    if (!content_type.empty())
        env.push_back("CONTENT_TYPE=" + content_type);

    // CONTENT_LENGTH
    const std::string& content_length = req.getHeader("content-length");
    if (!content_length.empty())
        env.push_back("CONTENT_LENGTH=" + content_length);


    // HTTP_ACCEPT
    const std::string& accept = req.getHeader("accept");
    if (!accept.empty())
        env.push_back("HTTP_ACCEPT=" + accept);

    // HTTP_USER_AGENT
    const std::string& user_agent = req.getHeader("user-agent");
    if (!user_agent.empty())
        env.push_back("HTTP_USER_AGENT=" + user_agent);
        
    // HTTP_REFERER
    const std::string& referer = req.getHeader("referer");
    if (!referer.empty())
        env.push_back("HTTP_REFERER=" + referer);
    
    // HTTP_COOKIE
    const std::string& cookie = req.getHeader("cookie");
    if (!cookie.empty())
        env.push_back("HTTP_COOKIE=" + cookie);
    
    return (env);
}


/*

request_uri = /tester.py/upload/dir
script_name = /tester.py
path_info = /upload/dir
path_translated = root + path_info
document_root = root

*/
