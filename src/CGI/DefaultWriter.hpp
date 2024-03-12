/*	                     __          _
 *	   __  ___________ _/ /___ ___  (_)
 *	  / / / / ___/ __ `/ / __ `__ \/ /
 *	 / /_/ (__  ) /_/ / / / / / / / / 
 *	 \__, /____/\__,_/_/_/ /_/ /_/_/ 
 *	/____/	User: Youssef Salmi
 *			File: DefaultWriter.hpp 
 */

#pragma once
#ifndef DEFAULTWRITER_HPP
#define DEFAULTWRITER_HPP

#include <iostream>

#include "IResponseWriter.hpp"

class DefaultWriter : public IResponseWriter
{
public:
	DefaultWriter();
	DefaultWriter( const DefaultWriter& d);
	~DefaultWriter();

	DefaultWriter&	operator=( const DefaultWriter& d );

};
#endif
