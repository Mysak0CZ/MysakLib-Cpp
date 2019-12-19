#include "mysakLib.h"
#include "logger.h"

#include <stdarg.h>
#include <time.h>


namespace MLib
{
MysakLib MysakLib::INSTANCE;

MysakLib::MysakLib()
{
	startTime = time(NULL);
	randSeed = startTime;
	loglevel = LOG_debug;
	logfile.open(M_LOGFILE_NAME, std::ofstream::out | std::ofstream::trunc);
#if defined INTERACTIVE && !defined _WIN
	tcgetattr(fileno(stdin), &(oldTerminos));
	tcgetattr(fileno(stdin), &(newTerminos));
	newTerminos.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL | IXON);
	newTerminos.c_lflag &= ~(ECHO | ECHONL | ICANON | ISIG | IEXTEN);
	newTerminos.c_cflag &= ~(CSIZE | PARENB);
	newTerminos.c_cflag |= CS8;
	makeRawConsole();
#endif
	Log(LOG_debug, true) << "MysakLib initialized.";
}

MysakLib::~MysakLib()
{
	Log(LOG_debug, true) << "MysakLib destroy";
	makeNormalConsole();
	logfile.close();
}

ulong_t MysakLib::randUInt(ulong_t min, ulong_t max)
{
	ulong_t oldSeed = randSeed;
	randSeed = ((1103515245ULL * randSeed) + 12345ULL) % (1ULL << 31);
	ulong_t res = (randSeed % (max - min)) + min;
	Log(LOG_verbose, true) << "randUInt " << oldSeed << " -> " << randSeed << ' ' << min << '~' << max << " = " << res;
	return res;
}

long MysakLib::randInt(long min, long max)
{
	ulong_t oldSeed = randSeed;
	randSeed = ((1103515245ULL * randSeed) + 12345ULL) % (1ULL << 31);
	long res = (randSeed % (max - min)) + min;
	Log(LOG_verbose, true) << "randInt " << oldSeed << " -> " << randSeed << ' ' << min << '~' << max << " = " << res;
	return res;
}

bool MysakLib::prob(ulong_t probability)
{
	ulong_t oldSeed = randSeed;
	randSeed = ((1103515245ULL * randSeed) + 12345ULL) % (1ULL << 31);
	bool result = (randSeed % 100) < probability ? true : false;
	Log(LOG_verbose, true) << "prob (" << probability << "%) " << oldSeed << " -> " << randSeed << " (" << (randSeed % 100) << ") = " << (result ? "true" : "false");
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