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
	constexpr vector2_t() :
	    vector2_t(0, 0){};
	constexpr vector2_t(vector2_t<T>& other)
	{
		set(other);
	}
	constexpr vector2_t(T sx, T sy)
	{
		x = sx;
		y = sy;
	}

	T x;
	T y;

	constexpr void set(vector2_t<T> const& other)
	{
		x = other.x;
		y = other.y;
	}

	void add(vector2_t<T> const& other)
	{
		x += other.x;
		y += other.y;
	}

	bool operator==(vector2_t<T> const& other)
	{
		return (other.x == x) && (other.y == y);
	}

	bool operator!=(vector2_t<T> const& other)
	{
		return (other.x != x) || (other.y != y);
	}
};
