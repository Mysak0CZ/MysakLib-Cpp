#pragma once

#include "types.h"

#ifndef M_LOGFILE_NAME
#	define M_LOGFILE_NAME "mLib.log"
#endif

#if defined INTERACTIVE && !defined _WIN
#	include <termios.h>
#endif

namespace MLib
{
const unsigned LOG_error = 0;
const unsigned LOG_warning = 1;
const unsigned LOG_info = 2;
const unsigned LOG_verbose = 3;
const unsigned LOG_debug = 4;
class Log;

class Logger final
{
	friend class Log;

   public:
	void setLoggingLevel(unsigned level)
	{
		LOGGER.level = level;
	}

   private:
	std::ofstream outfile;
	static Logger LOGGER;
	unsigned level = LOG_debug;

	Logger();

	~Logger();

	template <typename T>
	Logger& operator<<(const T& msg)
	{
		outfile << msg;
		outfile.flush();
		return *this;
	}
};

class Log
{
   private:
	static void writeTypeString(unsigned type)
	{
		switch (type) {
			case LOG_error:
				Logger::LOGGER << "[ERROR] ";
				break;
			case LOG_warning:
				Logger::LOGGER << "[WARN]  ";
				break;
			case LOG_info:
				Logger::LOGGER << "[INFO]  ";
				break;
			case LOG_verbose:
				Logger::LOGGER << "[VERB]  ";
				break;
			case LOG_debug:
				Logger::LOGGER << "[DEBUG] ";
				break;
			default:
				Logger::LOGGER << "[?????] ";
				break;
		}
	}

	unsigned level;

   public:
	Log() :
	    Log(LOG_info) {}

	Log(unsigned type) :
	    Log(type, false)
	{
	}

	Log(unsigned type, bool internal);


	~Log();

	template <typename T>
	Log& operator<<(const T& msg)
	{
		if (level <= Logger::LOGGER.level)
			Logger::LOGGER << msg;
		return *this;
	}
};


}  // namespace MLib

#define LOG_ERR ::MLib::Log(::MLib::LOG_error)
#define LOG_WARN ::MLib::Log(::MLib::LOG_warning)
#define LOG_INFO ::MLib::Log(::MLib::LOG_info)
#define LOG_VERB ::MLib::Log(::MLib::LOG_verbose)
#define LOG_DEBUG ::MLib::Log(::MLib::LOG_debug)
#define LOG ::MLib::Log()
