#pragma once

#include "types.h"

#if defined INTERACTIVE && !defined _WIN
#	include <termios.h>
#endif

#ifndef M_LOGFILE_NAME
#	define M_LOGFILE_NAME "mLib.log"
#endif

namespace MLib
{
class Log;

class MysakLib
{
	friend class Log;

   public:
	static MysakLib INSTANCE;

	unsigned loglevel;

   private:
	ulong_t randSeed;
	ulong_t startTime;
#if defined INTERACTIVE && !defined _WIN
	termios oldTerminos;
	termios newTerminos;
#endif

	std::ofstream logfile;

	MysakLib();
	~MysakLib();

   public:
	/* random uint */
	ulong_t randUInt(ulong_t min, ulong_t max);

	/* random int */
	long randInt(long min, long max);

	/* Probability in percent */
	bool prob(ulong_t probability);

	void makeRawConsole();
	void makeNormalConsole();
};
}  // namespace MLib

#define mLib ::MLib::MysakLib::INSTANCE
