#pragma once

#include "types.h"

#if defined INTERACTIVE && !defined _WIN
#	include <termios.h>
#endif


namespace MLib
{
class MysakLib
{
   public:
	static MysakLib INSTANCE;

   private:
	ulong_t randSeed;
	ulong_t startTime;
#if defined INTERACTIVE && !defined _WIN
	termios oldTerminos;
	termios newTerminos;
#endif

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
