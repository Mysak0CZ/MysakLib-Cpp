#pragma once

#include "types.h"

namespace MLib
{
/* Wait for X milliseconds */
void wait(ulong_t milliseconds);

/* Get size of the console (real, not buffer) */
vector2_t<long> getConsoleSize();

/* set cursor position in console (top-left <=> 0, 0) */
void setConsolePos(long x, long y);

void setConsoleFgColor(long color);
void setConsoleBgColor(long color);


/* Count target in string str */
ulong_t strCountChar(std::string& str, char target);

template <typename... Args>
std::string string_format(const std::string& format, Args... args);

}  // namespace MLib