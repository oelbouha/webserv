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

ProxyPair    CGIHandler::handle(Request* request, const std::string& path)
{
    Logger::debug ("handling cgi").flush();
    std::string file = path;
    
    int input[2];
    int output[2];

    int err = pipe(input);
    if (err)
        return ProxyPair();
    err = pipe(output);
    if (err)
        return ProxyPair();

    int pid = fork();
    if (pid < 0)
        return ProxyPair();
    
    if (pid == 0)
    {
        std::vector<std::string> args, env = compileEnv(*request);

        close(input[1]);
        close(output[0]);
        dup2(input[0], 0);
        dup2(output[1], 1);
        close(input[0]);
        close(output[1]);

        err = chdir(path.substr(0, path.rfind("/") + 1).c_str());
        if (err)
            exit(1);

        args.push_back(file);
        execve(
            file.c_str(),
            utils::vector_to_cstring_array(args),
            utils::vector_to_cstring_array(env)
        );
        std::perror("cgi");
        std::exit(1);
    }

    close(input[0]);
    close(output[1]);

    IProxyRequest*  req = new CGIRequest(input[1], *request);
    IProxyResponse* res = new CGIResponse(output[0], request);
    ProxyPair      ret(pid, req, res);
    
    return (ret);
}

std::vector<std::string>    CGIHandler::compileEnv(Request& req)
{
    std::vector<std::string> env;
    env.push_back("GATEWAY_INTERFACE=CGI/1.1");
    env.push_back("SERVER_SOFTWARE=webserv/1.0");
    env.push_back("SERVER_PROTOCOL=HTTP/1.1");
    env.push_back("SERVER_NAME=" + req.getHeader("host"));
    env.push_back("SERVER_PORT=" + utils::to_string(req.incomingPort));
    env.push_back("REQUEST_METHOD=" + req.getMethod());
    env.push_back("REQUEST_URI=" + req.getURI());
    env.push_back("PATH_INFO=" + req.getURI());
    env.push_back("SCRIPT_NAME=" + req.getURI());
    if (req.getQuery().empty() == false)
        env.push_back("QUERY_STRING=" + req.getQuery());

    const std::string& content_type = req.getHeader("content-type");
    if (!content_type.empty())
        env.push_back("CONTENT_TYPE=" + content_type);

    const std::string& content_length = req.getHeader("content-length");
    if (!content_length.empty())
        env.push_back("CONTENT_LENGTH=" + content_length);

    const std::string& accept = req.getHeader("accept");
    if (!accept.empty())
        env.push_back("HTTP_ACCEPT=" + accept);

    const std::string& user_agent = req.getHeader("user-agent");
    if (!user_agent.empty())
        env.push_back("HTTP_USER_AGENT=" + user_agent);
        
    const std::string& referer = req.getHeader("referer");
    if (!referer.empty())
        env.push_back("HTTP_REFERER=" + referer);
    
    const std::string& cookie = req.getHeader("cookie");
    if (!cookie.empty())
        env.push_back("HTTP_COOKIE=" + cookie);
    
    return (env);
}
