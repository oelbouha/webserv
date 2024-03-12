/*	                     __          _
 *	   __  ___________ _/ /___ ___  (_)
 *	  / / / / ___/ __ `/ / __ `__ \/ /
 *	 / /_/ (__  ) /_/ / / / / / / / / 
 *	 \__, /____/\__,_/_/_/ /_/ /_/_/ 
 *	/____/	User: Youssef Salmi
 *			File: ChunkedWriter.hpp 
 */

#pragma once
#ifndef CHUNKEDWRITER_HPP
#define CHUNKEDWRITER_HPP

#include <iostream>

#include "IResponseWriter.hpp"

class ChunkedWriter : public IResponseWriter
{
public:
	ChunkedWriter();
	ChunkedWriter( const ChunkedWriter& c);
	~ChunkedWriter();

	ChunkedWriter&	operator=( const ChunkedWriter& c );

};
#endif
