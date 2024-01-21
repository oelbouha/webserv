/*	                     __          _
 *	   __  ___________ _/ /___ ___  (_)
 *	  / / / / ___/ __ `/ / __ `__ \/ /
 *	 / /_/ (__  ) /_/ / / / / / / / / 
 *	 \__, /____/\__,_/_/_/ /_/ /_/_/ 
 *	/____/	User: Youssef Salmi
 *			File: IServer.hpp 
 */

#pragma once
#ifndef ISERVER_HPP
#define ISERVER_HPP

#include "src/Response/Response.hpp"
#include "src/Response/ProxiedResponse.hpp"
#include "src/Request/Request.hpp"
#include "src/DataTypes/Config.hpp"
#include "Utils.hpp"

class IServer
{
    public:
        IServer();
        virtual	~IServer();
        
        void	    addInlineIfNotExist(Config& route, Config& server, const string& prop);
        void		setInlineIfExist(Config &, string & , const string & );
        void		setBlockIfExist(Config &, std::vector<Config*> &, const string& );
        void		setListIfExist(Config &, std::vector<string> &, const string & );
        void	    addInLineIfExist(Config& route, Config& server, const string& prop);
	    void	    addBlockIfExist(Config& route, Config& server, string prop);
	    void	    addListIfExist(Config& route, Config& server, const string& prop);
        void		setupErrorPages();
    
        void	    printErrorPages();
    
        bool	    IsDirectory(string uri);
        bool		IsErrorCodeInErrorPages(unsigned int );

        virtual IResponse*  handle( const IRequest& req ) = 0;
        virtual string      getRoot() const = 0;
        IResponse*	        GenerateErrorPageResponse(const IRequest& request, unsigned in );
        
        string	    getMimeType(const string& uri);
        string	    getExtension(string line) const;
        string	    getStatusCodeResourceFile(unsigned int stsCode);

    protected:
        std::vector<std::pair<string, std::vector<string> > >	error_pages;
        std::vector<Config*>	error_page;
        string root;
};

#endif
