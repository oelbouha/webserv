/*	                     __          _
 *	   __  ___________ _/ /___ ___  (_)
 *	  / / / / ___/ __ `/ / __ `__ \/ /
 *	 / /_/ (__  ) /_/ / / / / / / / / 
 *	 \__, /____/\__,_/_/_/ /_/ /_/_/ 
 *	/____/	User: Youssef Salmi
 *			File: ILogger.hpp 
 */

#pragma once
#ifndef ILOGGER_HPP
#define ILOGGER_HPP

#include <string>

enum LogLevel
{

};

class ILogger
{
	public:
		//virtual ;


		ILogger&	operator<<(LogLevel& aLevel);
		ILogger&	operator<<(const std::string& aMessage);
		ILogger&	operator<<(long int& aParam);
		ILogger&	operator<<(double& aParam);
		virtual		~ILogger(){};

};

/*

Logger::info("klsdfksdf %s %d\n", var, a);

logger << LogLevel::info << "message" << a << std::endl;

*/
#endif