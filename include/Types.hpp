/*	                     __          _
 *	   __  ___________ _/ /___ ___  (_)
 *	  / / / / ___/ __ `/ / __ `__ \/ /
 *	 / /_/ (__  ) /_/ / / / / / / / / 
 *	 \__, /____/\__,_/_/_/ /_/ /_/_/ 
 *	/____/	User: Youssef Salmi
 *			File: Types.hpp 
 */

#pragma once
#ifndef TYPES_HPP
#define TYPES_HPP

#include <iostream>

typedef unsigned int ip_t;

typedef enum e_method
{
    HEAD,
    GET,
    POST,
    PUT,
    DELETE,
    UNDEFINED
} method_t;

#endif
