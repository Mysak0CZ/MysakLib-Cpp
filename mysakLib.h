#pragma once

#include "types.h"

#ifndef M_LOGFILE_NAME
#	define M_LOGFILE_NAME "mLib.log"
#endif

#define LOG(...)

namespace MLib
{
enum LOGLEVEL : int
{
	error = 0,
	warning = 1,
	info = 2,
	debug = 3
};

namespace
{
	class MysakLib
	{
	   private:
		ulong_t randSeed;
		FILE* logfile;
		ulong_t startTime;

	   public:
		MysakLib();
		~MysakLib();

		LOGLEVEL logLevel = LOGLEVEL::debug;
		LOGLEVEL internalLogLevel = LOGLEVEL::debug;

		/* random uint */
		ulong_t randUInt(ulong_t min, ulong_t max);

		/* random int */
		long randInt(long min, long max);

		/* Probability in percent */
		bool prob(ulong_t probability);

		/* log error into logfile */
		void logError(const char* format, ...);

		/* log warning into logfile */
		void logWarning(const char* format, ...);

		/* log info into logfile */
		void logInfo(const char* format, ...);

		/* log debug message into logfile */
		void logDebug(const char* format, ...);

		void makeRawConsole();
		void makeNormalConsole();
	};
}  // namespace
}  // namespace MLib
extern MLib::MysakLib mLib;

#define LOG_ERROR(...) ::mLib.logError(__VA_ARGS__)
#define LOG_WARNING(...) ::mLib.logWarning(__VA_ARGS__)
#define LOG_INFO(...) ::mLib.logInfo(__VA_ARGS__)
#define LOG_DEBUG(...) ::mLib.logDebug(__VA_ARGS__)
