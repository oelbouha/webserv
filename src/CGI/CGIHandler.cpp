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

ProxyPair    CGIHandler::handle(IRequest* request) // root
{
    std::string file = "./" + request->getURI().substr(request->getURI().rfind("/") + 1);
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

        std::cout << "file: " << file << std::endl;
        mArgs.push_back(file);

        // build env;
        compileEnv(*request);

        // excute script;
        execve(file.c_str(), vectorToCStringArray(mArgs), vectorToCStringArray(mEnv));
        perror("cgi");
        exit(1);
    }

    // main process - webserv
    //
    close(input[0]);
    close(output[1]);

    IProxyRequest*  req = new DescriptorProxyRequest(input[1], *request);
    IProxyResponse* res = new CGIResponse(output[0], request->getSocket());
    std::cout << pid << " - " << req << " - " << res << std::endl;
    ProxyPair      ret(pid, req, res);
    
    return (ret);
}

void    CGIHandler::compileEnv(IRequest& req)
{
    // GATEWAY_INTERFACE
    mEnv.push_back("GATEWAY_INTERFACE=CGI/1.1");
    // SERVER_SOFTWARE
    mEnv.push_back("SERVER_SOFTWARE=webserv/1.0");
    // SERVER_PROTOCOL
    mEnv.push_back("SERVER_PROTOCOL=HTTP/1.1");
    // SERVER_NAME
    mEnv.push_back("SERVER_NAME=" + req.getHeader("host"));
    // SERVER_PORT
    mEnv.push_back("SERVER_PORT=" + std::to_string(req.getIncomingPort()));
    // REQUEST_METHOD
    mEnv.push_back("REQUEST_METHOD=" + req.getMethod());
    // REQUEST_URI
    mEnv.push_back("REQUEST_URI=" + req.getURI());
    // PATH_INFO
    mEnv.push_back("PATH_INFO=" + req.getURI());
    // PATH_TRANSLATED
    // mEnv.push_back("PATH_TRANSLATED=/Users/ysalmi/code/in_progress/webserv/pages/tester.py");
    // SCRIPT_NAME
    mEnv.push_back("SCRIPT_NAME=" + req.getURI());
    // DOCUMENT_ROOT
    // mEnv.push_back("DOCUMENT_ROOT=/Users/ysalmi/code/in_progress/webserv/pages");
    // QUERY_STRING
    if (req.getQuery().empty() == false)
        mEnv.push_back("QUERY_STRING=" + req.getQuery());

    // CONTENT_TYPE
    const std::string& content_type = req.getHeader("content-type");
    if (!content_type.empty())
        mEnv.push_back("CONTENT_TYPE=" + content_type);

    // CONTENT_LENGTH
    const std::string& content_length = req.getHeader("content-length");
    if (!content_length.empty())
        mEnv.push_back("CONTENT_LENGTH=" + content_length);


    // HTTP_ACCEPT
    const std::string& accept = req.getHeader("accept");
    if (!accept.empty())
        mEnv.push_back("HTTP_ACCEPT=" + accept);

    // HTTP_USER_AGENT
    const std::string& user_agent = req.getHeader("user-agent");
    if (!user_agent.empty())
        mEnv.push_back("HTTP_USER_AGENT=" + user_agent);
        
    // HTTP_REFERER
    const std::string& referer = req.getHeader("referer");
    if (!referer.empty())
        mEnv.push_back("HTTP_REFERER=" + referer);
    
    // HTTP_COOKIE
    const std::string& cookie = req.getHeader("cookie");
    if (!cookie.empty())
        mEnv.push_back("HTTP_COOKIE=" + cookie);
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
