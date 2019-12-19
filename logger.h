#pragma once

#include "mysakLib.h"
#include "types.h"

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

class Log
{
   private:
	static void writeTypeString(unsigned type)
	{
		switch (type) {
			case LOG_error:
				MysakLib::INSTANCE.logfile << "[ERROR] ";
				break;
			case LOG_warning:
				MysakLib::INSTANCE.logfile << "[WARN]  ";
				break;
			case LOG_info:
				MysakLib::INSTANCE.logfile << "[INFO]  ";
				break;
			case LOG_verbose:
				MysakLib::INSTANCE.logfile << "[VERB]  ";
				break;
			case LOG_debug:
				MysakLib::INSTANCE.logfile << "[DEBUG] ";
				break;
			default:
				MysakLib::INSTANCE.logfile << "[?????] ";
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
		if (level <= MysakLib::INSTANCE.loglevel)
			MysakLib::INSTANCE.logfile << msg;
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
