/*	                     __          _
 *	   __  ___________ _/ /___ ___  (_)
 *	  / / / / ___/ __ `/ / __ `__ \/ /
 *	 / /_/ (__  ) /_/ / / / / / / / / 
 *	 \__, /____/\__,_/_/_/ /_/ /_/_/ 
 *	/____/	User: Youssef Salmi
 *			File: SelectMultiplexer.hpp 
 */

#pragma once
#ifndef SELECTMULTIPLEXER_HPP
#define SELECTMULTIPLEXER_HPP

#include <iostream>

#include "IMultiplexer.hpp"

class SelectMultiplexer : public IMultiplexer
{
public:
	SelectMultiplexer();
	SelectMultiplexer( const SelectMultiplexer& s);
	~SelectMultiplexer();

	SelectMultiplexer&	operator=( const SelectMultiplexer& s );

};
#endif
