#pragma once

#if defined _WIN32 || defined _WIN64
#	define _WIN
#else
#endif

#ifndef DISABLE_INTERACTIVE
#	define INTERACTIVE
#endif

#include <bits/stdc++.h>

typedef unsigned int uint_t;
typedef unsigned long ulong_t;
typedef long long llong_t;
typedef unsigned long long ullong_t;

template <typename T>
class vector2_t
{
   public:
	T x;
	T y;
};
