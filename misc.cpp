#include "misc.h"

#ifdef _WIN
#	include <Windows.h>
#else
#	include <time.h>
#	include <sys/ioctl.h>
#endif

namespace MLib
{
/* Wait for X milliseconds */
void wait(ulong_t milliseconds)
{
	fflush(stdout);
#ifndef DISABLE_TIMING
#	ifdef _WIN
	Sleep(milliseconds);
#	else
	struct timespec ts;
	ts.tv_sec = milliseconds / 1000;
	ts.tv_nsec = (milliseconds % 1000) * 1000000;
	nanosleep(&ts, NULL);
#	endif
#endif
}

/* Get size of the console (real, not buffer) */
vector2_t<long> getConsoleSize()
{
	vector2_t<long> result;
#ifdef _WIN
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
	result.x = csbi.srWindow.Right - csbi.srWindow.Left + 1;
	result.y = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
#else
	struct winsize win;
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &win);
	result.x = win.ws_col;
	result.y = win.ws_row;
#endif
	return result;
}

/* set cursor position in console (top-left <=> 0, 0) */
void setConsolePos(long x, long y)
{
#ifdef _WIN
	COORD coord;  // coordinates
	coord.X = x;
	coord.Y = y;  // X and Y coordinates
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
#else
	fprintf(stdout, "%c[%ld;%ldf", 0x1B, y + 1, x + 1);
#endif
}

/* Count target in string str */
ulong_t strCountChar(std::string& str, char target)
{
	ulong_t result = 0;
	for (char c : str) {
		if (c == target)
			result++;
	}
	return result;
}

}  // namespace MLib