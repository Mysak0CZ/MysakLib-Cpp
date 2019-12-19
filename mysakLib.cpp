#include "mysakLib.h"

#include <stdarg.h>
#include <time.h>


namespace MLib
{
MysakLib MysakLib::INSTANCE;

MysakLib::MysakLib()
{
	startTime = time(NULL);
	randSeed = startTime;
#if defined INTERACTIVE && !defined _WIN
	tcgetattr(fileno(stdin), &(oldTerminos));
	tcgetattr(fileno(stdin), &(newTerminos));
	newTerminos.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL | IXON);
	newTerminos.c_lflag &= ~(ECHO | ECHONL | ICANON | ISIG | IEXTEN);
	newTerminos.c_cflag &= ~(CSIZE | PARENB);
	newTerminos.c_cflag |= CS8;
	makeRawConsole();
#endif
	//logDebug("MysakLib: initialized");
}

MysakLib::~MysakLib()
{
	//logDebug("MysakLib: delete");
	makeNormalConsole();
}

ulong_t MysakLib::randUInt(ulong_t min, ulong_t max)
{
	ulong_t oldSeed = randSeed;
	randSeed = ((1103515245ULL * randSeed) + 12345ULL) % (1ULL << 31);
	//logInfo("randUInt %lu -> %lu %lu~%lu = %lu", oldSeed, randSeed, min, max, (randSeed % (max - min)) + min);
	return (randSeed % (max - min)) + min;
}

long MysakLib::randInt(long min, long max)
{
	ulong_t oldSeed = randSeed;
	randSeed = ((1103515245ULL * randSeed) + 12345ULL) % (1ULL << 31);
	//logInfo("ranInt %lu -> %lu %ld~%ld = %ld", oldSeed, randSeed, min, max, (randSeed % (max - min)) + min);
	return (randSeed % (max - min)) + min;
}

bool MysakLib::prob(ulong_t probability)
{
	ulong_t oldSeed = randSeed;
	randSeed = ((1103515245ULL * randSeed) + 12345ULL) % (1ULL << 31);
	bool result = (randSeed % 100) < probability ? true : false;
	//logInfo("prob (%lu%%) %lu -> %lu (%lu%%) = %s", probability, oldSeed, randSeed, (randSeed % 100), result ? "true" : "false");
	return result;
}

void MysakLib::makeRawConsole()
{
#if defined INTERACTIVE && !defined _WIN
	tcsetattr(fileno(stdin), TCSANOW, &(newTerminos));
#endif
}

void MysakLib::makeNormalConsole()
{
#if defined INTERACTIVE && !defined _WIN
	tcsetattr(fileno(stdin), TCSANOW, &(oldTerminos));
#endif
}

}  // namespace MLib