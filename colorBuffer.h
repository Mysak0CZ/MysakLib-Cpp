#pragma once

#include <bits/stdc++.h>
#include "types.h"

namespace MLib
{
enum Color : uint8_t
{
	Black = 0,
	Red = 1,
	Green = 2,
	Yellow = 3,
	Blue = 4,
	Magenta = 5,
	Cyan = 6,
	White = 7,
	BrightBlack = 8,
	BrightRed = 9,
	BrightGreen = 10,
	BrightYellow = 11,
	BrightBlue = 12,
	BrightMagenta = 13,
	BrightCyan = 14,
	BrightWhite = 15,
};

constexpr uint8_t
colorFromRGB(double r, double g, double b)
{
	return 16 + static_cast<int>(36 * 5 * r) + static_cast<int>(6 * 5 * g) + static_cast<int>(5 * b);
}

constexpr uint8_t
colorFromRGB(int r, int g, int b)
{
	return colorFromRGB(r / 255., g / 255., b / 255.);
}

constexpr inline uint32_t makePack(uint8_t bc, uint8_t fc, char c)
{
	return (bc << 16) | (fc << 8) | static_cast<uint8_t>(c);
}

constexpr inline uint8_t packGetBColor(uint32_t pack)
{
	return (pack >> 16) & 0xFF;
}

constexpr inline uint8_t packGetFColor(uint32_t pack)
{
	return (pack >> 8) & 0xFF;
}

constexpr inline char packGetChar(uint32_t pack)
{
	return static_cast<char>(pack & 0xFF);
}

class ColorBuffer
{
   public:
	static constexpr uint32_t BLANK = makePack(Color::Black, Color::BrightWhite, ' ');
	static constexpr uint32_t EMPTY = 0;

   private:
	long size_x;
	long size_y;
	bool needUpdate = true;
	std::vector<uint32_t> frame;
	std::vector<uint32_t> nxframe;

   public:
	ColorBuffer();
	~ColorBuffer();

	void draw();
	void redraw();
	bool set(long x, long y, uint8_t colorB, uint8_t colorF, char c);
	bool set(long x, long y, uint32_t pack);

	void resize(long x, long y);
	void clear();
	void getSize(vector2_t<long>& target) const;
};
}  // namespace MLib