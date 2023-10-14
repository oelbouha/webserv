/*	                     __          _
 *	   __  ___________ _/ /___ ___  (_)
 *	  / / / / ___/ __ `/ / __ `__ \/ /
 *	 / /_/ (__  ) /_/ / / / / / / / / 
 *	 \__, /____/\__,_/_/_/ /_/ /_/_/ 
 *	/____/	User: Youssef Salmi
 *			File: ConnectionsPool.hpp 
 */

#pragma once

#include <iostream>

class ConnectionsPool
{
	/*	Private members here	*/


	protected:

	public:
		ConnectionsPool( void );
		ConnectionsPool( ConnectionsPool& c );
		~ConnectionsPool();

		ConnectionsPool&	operator=( ConnectionsPool& c );

};
