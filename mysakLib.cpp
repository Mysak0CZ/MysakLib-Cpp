#include "mysakLib.h"

#include <stdarg.h>
#include <time.h>

MLib::MysakLib mLib;

namespace MLib
{
namespace
{
	MysakLib::MysakLib()
	{
		startTime = time(NULL);
		randSeed = startTime;
		logfile = fopen(M_LOGFILE_NAME, "w");
		if (logfile == NULL) {
			throw std::invalid_argument("Failed to open logfile");
		}
#if defined INTERACTIVE && !defined _WIN
		tcgetattr(STDIN_FILENO, &(MysakLib_internals_mlib.oldTerminos));
		tcgetattr(STDIN_FILENO, &(MysakLib_internals_mlib.newTerminos));
		MysakLib_internals_mlib.newTerminos.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL | IXON);
		MysakLib_internals_mlib.newTerminos.c_lflag &= ~(ECHO | ECHONL | ICANON | ISIG | IEXTEN);
		MysakLib_internals_mlib.newTerminos.c_cflag &= ~(CSIZE | PARENB);
		MysakLib_internals_mlib.newTerminos.c_cflag |= CS8;
		makeRawConsole();
#endif
		logDebug("MysakLib: initialized");
	}

	MysakLib::~MysakLib()
	{
		logDebug("MysakLib: delete");
		fclose(logfile);
		makeNormalConsole();
	}

	ulong_t MysakLib::randUInt(ulong_t min, ulong_t max)
	{
		ulong_t oldSeed = randSeed;
		randSeed = ((1103515245ULL * randSeed) + 12345ULL) % (1ULL << 31);
		logInfo("randUInt %lu -> %lu %lu~%lu = %lu", oldSeed, randSeed, min, max, (randSeed % (max - min)) + min);
		return (randSeed % (max - min)) + min;
	}

	long MysakLib::randInt(long min, long max)
	{
		ulong_t oldSeed = randSeed;
		randSeed = ((1103515245ULL * randSeed) + 12345ULL) % (1ULL << 31);
		logInfo("ranInt %lu -> %lu %ld~%ld = %ld", oldSeed, randSeed, min, max, (randSeed % (max - min)) + min);
		return (randSeed % (max - min)) + min;
	}

	bool MysakLib::prob(ulong_t probability)
	{
		ulong_t oldSeed = randSeed;
		randSeed = ((1103515245ULL * randSeed) + 12345ULL) % (1ULL << 31);
		bool result = (randSeed % 100) < probability ? true : false;
		logInfo("prob (%lu%%) %lu -> %lu (%lu%%) = %s", probability, oldSeed, randSeed, (randSeed % 100), result ? "true" : "false");
		return result;
	}

	void MysakLib::logError(const char* format, ...)
	{
		char buffer[1025];
		va_list args;
		va_start(args, format);
		vsnprintf(buffer, 1024, format, args);
		if (logLevel >= LOGLEVEL::error) {
			buffer[1024] = '\0';
			fprintf(logfile != NULL ? logfile : stderr, "%5ld   ERROR %s\n", time(NULL) - startTime, buffer);
		}
		va_end(args);
		if (logfile != NULL)
			fflush(logfile);
	}

	void MysakLib::logWarning(const char* format, ...)
	{
		char buffer[1025];
		va_list args;
		va_start(args, format);
		vsnprintf(buffer, 1024, format, args);
		if (logLevel >= LOGLEVEL::warning) {
			buffer[1024] = '\0';
			fprintf(logfile != NULL ? logfile : stderr, "%5ld   WARN  %s\n", time(NULL) - startTime, buffer);
		}
		va_end(args);
		if (logfile != NULL)
			fflush(logfile);
	}

	void MysakLib::logInfo(const char* format, ...)
	{
		char buffer[1025];
		va_list args;
		va_start(args, format);
		vsnprintf(buffer, 1024, format, args);
		if (logLevel >= LOGLEVEL::info) {
			buffer[1024] = '\0';
			fprintf(logfile != NULL ? logfile : stderr, "%5ld   INFO  %s\n", time(NULL) - startTime, buffer);
		}
		va_end(args);
		if (logfile != NULL)
			fflush(logfile);
	}

	void MysakLib::logDebug(const char* format, ...)
	{
		char buffer[1025];
		va_list args;
		va_start(args, format);
		vsnprintf(buffer, 1024, format, args);
		if (logLevel >= LOGLEVEL::debug) {
			buffer[1024] = '\0';
			fprintf(logfile != NULL ? logfile : stderr, "%5ld   DEBUG %s\n", time(NULL) - startTime, buffer);
		}
		va_end(args);
		if (logfile != NULL)
			fflush(logfile);
	}

	void MysakLib::makeRawConsole()
	{
#if defined INTERACTIVE && !defined _WIN
		tcsetattr(STDIN_FILENO, TCSANOW, &(MysakLib_internals_mlib.newTerminos));
#endif
	}

	void MysakLib::makeNormalConsole()
	{
#if defined INTERACTIVE && !defined _WIN
		tcsetattr(STDIN_FILENO, TCSANOW, &(MysakLib_internals_mlib.oldTerminos));
#endif
	}

}  // namespace
}  // namespace MLib