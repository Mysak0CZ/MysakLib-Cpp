#include "interface.h"
#include "mysakLib.h"

#include <bits/stdc++.h>

#ifdef _WIN
#	include <conio.h>
#else
#	include <sys/ioctl.h>
#endif

namespace MLib
{
bool askYN(std::string& question, bool plain = false)
{
#ifndef INTERACTIVE
	plain = true;
#endif
	char answer;
	std::cout << question << " [y/n]: ";
	for (;;) {
		if (plain) {
			mLib.makeNormalConsole();
			answer = tolower(getCharB());
		} else {
			answer = tolower(getCharNB());
		}
		if (answer == 'y') {
			mLib.logInfo("askYN \"%s\": true", question.c_str());
			if (plain) {
				getCharNB();
				mLib.makeRawConsole();
			} else
				std::cout << '\n';
			return true;
		}
		if (answer == 'n') {
			mLib.logInfo("askYN \"%s\": false", question.c_str());
			if (plain) {
				getCharNB();
				mLib.makeRawConsole();
			} else
				std::cout << '\n';
			return false;
		}
		if (plain) {
			std::cout << "Invalid option!\n"
			          << question << " [y/n]: ";
		}
	}
}

long readInt(std::string& query, bool plain = false)
{
#ifndef INTERACTIVE
	plain = true;
#endif
	long num = 0;
	bool negative = false;
	char c;
	if (plain) {
		mLib.makeNormalConsole();
		std::cout << query << "> ";
		fflush(stdout);
		while (scanf("%ld", &num) != 1)
			getCharB();
		mLib.makeRawConsole();
	} else {
		for (;;) {
			std::cout << '\r' << query << "> " << (negative ? '-' : ' ') << num << "   ";
			fflush(stdout);
			c = getCharB();
			if (c == '-') {
				negative = !negative;
			} else if (c == '\r' || c == '\n') {
				std::cout << '\n';
				break;
			} else if (c == '\b') {
				num /= 10;
			} else if (c >= '0' && c <= '9') {
				num *= 10;
				num += c - '0';
			}
		}
		if (negative)
			num = -num;
	}
	mLib.logInfo("readInt \"%s\": %ld", query.c_str(), num);
	return num;
}

std::string readStr(std::string& query, size_t maxLen, bool plain = false)
{
#ifndef INTERACTIVE
	plain = true;
#endif
	return "";
}

long ioSelection(std::string& title, std::vector<std::string>& options, bool plain = false)
{
#ifndef INTERACTIVE
	plain = true;
#endif
	return 0;
}

int getCharB()
{
	int res;
#ifdef _WIN
#	ifdef INTERACTIVE
	res = _getch();
	if (res == 0 || res == 0xe0) {  // Multichar command
		res <<= 8;
		res |= _getch();
	}
#	else
	res = getchar();
	if (res == 0 || res == 0xe0) {  // Multichar command
		res <<= 8;
		res |= getchar();
	}
#	endif
#else
	int chr;

	res = getchar();
	if (res == 0x1b) {  // ESC sequence
		res = getchar();
		if (res != '[') {
			MysakLib_internals_logError("Escape sequence 2nd char is not '[': %x", res);
			return 0;
		}
		res = 0x1b5b;
		do {
			chr = getchar();
			res = ((res << 8) & ~0xff) | chr;
		} while (chr < 0x40 || chr > 0x7E);
	}
#endif
	return res;
}

int getCharNB()
{
#ifdef _WIN
	if (!kbhit())
		return 0;
#else
	int n;
	if (ioctl(STDIN_FILENO, FIONREAD, &n) != 0 || n <= 0)
		return 0;
#endif
	return getCharB();
}

void clearConsole()
{
}
}  // namespace MLib
