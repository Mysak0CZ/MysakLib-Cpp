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
#ifdef INTERACTIVE
#	ifndef _WIN
	tcgetattr(fileno(stdin), &(oldTerminos));
	tcgetattr(fileno(stdin), &(newTerminos));
	newTerminos.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL | IXON);
	newTerminos.c_lflag &= ~(ECHO | ECHONL | ICANON | ISIG | IEXTEN);
	newTerminos.c_cflag &= ~(CSIZE | PARENB);
	newTerminos.c_cflag |= CS8;
#	else
#		ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
#			define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
#		endif
	// Set output mode to handle virtual terminal sequences
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	if (hOut == INVALID_HANDLE_VALUE) {
		Log(LOG_error, true) << "GetStdHandle failed with: " << GetLastError();
	} else {
		DWORD dwMode = 0;
		if (!GetConsoleMode(hOut, &dwMode)) {
			Log(LOG_error, true) << "GetConsoleMode failed with: " << GetLastError();
		} else {
			dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
			if (!SetConsoleMode(hOut, dwMode)) {
				Log(LOG_error, true) << "SetConsoleMode failed with: " << GetLastError();
			}
		}
	}
	HANDLE hIn = GetStdHandle(STD_INPUT_HANDLE);
	if (hIn == INVALID_HANDLE_VALUE) {
		Log(LOG_error, true) << "GetStdHandle failed with: " << GetLastError();
	} else {
		if (!GetConsoleMode(hOut, &oldDwMode)) {
			Log(LOG_error, true) << "GetConsoleMode failed with: " << GetLastError();
		}
	}
#	endif
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
#ifdef INTERACTIVE
#	ifdef _WIN
	HANDLE hIn = GetStdHandle(STD_INPUT_HANDLE);
	if (hIn == INVALID_HANDLE_VALUE) {
		Log(LOG_error, true) << "GetStdHandle failed with: " << GetLastError();
	} else {
		DWORD newMode = oldDwMode;
		newMode &= ~(ENABLE_ECHO_INPUT | ENABLE_LINE_INPUT);
		newMode |= ENABLE_MOUSE_INPUT;
		if (!SetConsoleMode(hOut, newMode)) {
			Log(LOG_error, true) << "SetConsoleMode failed with: " << GetLastError();
		}
	}
#	else
	tcsetattr(fileno(stdin), TCSANOW, &(newTerminos));
#	endif
#endif
}

void MysakLib::makeNormalConsole()
{
#ifdef INTERACTIVE
#	ifdef _WIN
	HANDLE hIn = GetStdHandle(STD_INPUT_HANDLE);
	if (hIn == INVALID_HANDLE_VALUE) {
		Log(LOG_error, true) << "GetStdHandle failed with: " << GetLastError();
	} else {
		if (!SetConsoleMode(hOut, oldDwMode)) {
			Log(LOG_error, true) << "SetConsoleMode failed with: " << GetLastError();
		}
	}
#	else
	tcsetattr(fileno(stdin), TCSANOW, &(oldTerminos));
#	endif
#endif
}

}  // namespace MLib
