/*	                     __          _
 *	   __  ___________ _/ /___ ___  (_)
 *	  / / / / ___/ __ `/ / __ `__ \/ /
 *	 / /_/ (__  ) /_/ / / / / / / / / 
 *	 \__, /____/\__,_/_/_/ /_/ /_/_/ 
 *	/____/	User: Youssef Salmi
 *			File: IResponseWriter.hpp 
 */

#pragma once
#ifndef IRESPONSEWRITER_HPP
#define IRESPONSEWRITER_HPP

class IResponseWriter
{
public:
	virtual		~IResponseWriter(){};

	virtual void		setHeader(const std::string& header) = 0;
	virtual void		append(const std::string& data) = 0;
	virtual int			write() = 0;
	virtual bool		done() const = 0;

};
#endif
