#pragma once

#include <bits/stdc++.h>
#include "types.h"

namespace MLib
{
class DoubleBuffer
{
   private:
	long size_x;
	long size_y;
	bool needUpdate = true;
	std::vector<char> frame;
	std::vector<char> nxframe;

   public:
	DoubleBuffer(long sizeX, long sizeY);
	~DoubleBuffer();

	void draw();
	void redraw();
	bool setCharAt(long x, long y, char c);
	// bool setCharAt(vector2_t pos, char c);

	void fillRect(long x1, long y1, long x2, long y2, char c);
	// void fillRect(vector2_t start, vector2_t size, char c);

	void loadFromString(std::string& str, long anchorx, long anchory);
};
}  // namespace MLib