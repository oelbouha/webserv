/*	                     __          _
 *	   __  ___________ _/ /___ ___  (_)
 *	  / / / / ___/ __ `/ / __ `__ \/ /
 *	 / /_/ (__  ) /_/ / / / / / / / / 
 *	 \__, /____/\__,_/_/_/ /_/ /_/_/ 
 *	/____/	User: Youssef Salmi
 *			File: main.cpp 
 */

#include <iostream>
#include <string>
#include <cstdio>

int main( int c, char *v[])
{
	if (c < 2) return 1;

	std::remove(v[1]);
	return (0);
}
