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
	    vector2_t(0, 0)
	{
	}
	constexpr vector2_t(vector2_t<T>& other) :
	    x(other.x), y(other.y)
	{
	}
	vector2_t(vector2_t<T>&& other) :
	    x(other.x), y(other.y)
	{
	}
	constexpr vector2_t(T x, T y) :
	    x(x), y(y)
	{
	}

	T x;
	T y;

	constexpr vector2_t<T>& set(vector2_t<T> const& other)
	{
		x = other.x;
		y = other.y;
		return *this;
	}

	constexpr vector2_t<T> operator-() const
	{
		return vector2_t<T>(-x, -y);
	}

	constexpr vector2_t<T> operator+(const vector2_t<T>& b) const
	{
		return vector2_t<T>(x + b.x, y + b.y);
	}

	constexpr vector2_t<T> operator-(const vector2_t<T>& b) const
	{
		return vector2_t<T>(x - b.x, y - b.y);
	}

	constexpr vector2_t<T> operator*(const T& b) const
	{
		return vector2_t<T>(x * b, y * b);
	}

	constexpr vector2_t<T> operator/(const T& b) const
	{
		return vector2_t<T>(x / b, y / b);
	}

	constexpr vector2_t<T>& operator=(const vector2_t<T>& b)
	{
		x = b.x;
		y = b.y;
		return *this;
	}

	constexpr vector2_t<T>& operator+=(const vector2_t<T>& b)
	{
		x += b.x;
		y += b.y;
		return *this;
	}

	constexpr vector2_t<T>& operator-=(const vector2_t<T>& b)
	{
		x -= b.x;
		y -= b.y;
		return *this;
	}

	constexpr vector2_t<T>& operator*=(const T& b)
	{
		x *= b;
		y *= b;
		return *this;
	}

	constexpr vector2_t<T>& operator/=(const T& b)
	{
		x /= b;
		y /= b;
		return *this;
	}

	constexpr bool operator==(const vector2_t<T>& other)
	{
		return (other.x == x) && (other.y == y);
	}

	constexpr bool operator!=(const vector2_t<T>& other)
	{
		return (other.x != x) || (other.y != y);
	}

	template <typename T2>
	operator vector2_t<T2>() const
	{
		return vector2_t<T2>(static_cast<T2>(x), static_cast<T2>(y));
	}
};
