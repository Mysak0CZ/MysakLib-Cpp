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

/* Count target in string str */
ulong_t strCountChar(std::string& str, char target);

}  // namespace MLib