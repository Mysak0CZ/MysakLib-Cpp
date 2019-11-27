#include "doubleBuffer.h"
#include "misc.h"

namespace MLib
{
DoubleBuffer::DoubleBuffer(long sizeX, long sizeY)
{
	frame.resize(sizeX * sizeY);
	nxframe.resize(sizeX * sizeY);
	size_x = sizeX;
	size_y = sizeY;
	for (long i = 0; i < sizeX * sizeY; i++) {
		frame[i] = ' ';
		nxframe[i] = ' ';
	}
}
DoubleBuffer::~DoubleBuffer()
{
}

void DoubleBuffer::draw()
{
	if (!needUpdate)
		return;
	for (long y = 0; y < size_y; y++) {
		bool lastPos = false;
		for (long x = 0; x < size_x; x++) {
			long pos = y * size_x + x;
			if (frame[pos] != nxframe[pos]) {
				if (!lastPos)
					setConsolePos(x, y);
				frame[pos] = nxframe[pos];
				putchar(nxframe[pos]);
				lastPos = true;
			} else {
				lastPos = false;
			}
		}
	}
	setConsolePos(0, size_y);
	fflush(stdout);
	needUpdate = false;
}

void DoubleBuffer::redraw()
{
	for (long i = 0; i < size_x * size_y; i++)
		frame[i] = '\0';
	needUpdate = true;
	draw();
}

bool DoubleBuffer::setCharAt(long x, long y, char c)
{
	if (x < 0 || y < 0 || x >= size_x || y >= size_y)
		return false;
	if (nxframe[y * size_x + x] != c) {
		nxframe[y * size_x + x] = c;
		needUpdate = true;
	}
	return true;
}

// bool DoubleBuffer::setCharAt(vector2_t pos, char c) {
// 	return setCharAt(pos)
// }

void DoubleBuffer::fillRect(long x1, long y1, long x2, long y2, char c)
{
	if (x1 < 0 || y1 < 0 || x2 < x1 || y2 < y1)
		return;
	if (x2 >= size_x)
		x2 = size_x - 1;
	if (y2 >= size_y)
		y2 = size_y - 1;

	for (long y = y1; y <= y2; y++) {
		for (long x = x1; x <= x2; x++) {
			setCharAt(x, y, c);
		}
	}
}
// void DoubleBuffer::fillRect(vector2_t start, vector2_t size, char c);

void DoubleBuffer::loadFromString(std::string& str, long anchorx, long anchory)
{
	if (anchory < 0 || anchorx < 0 || anchory >= size_y || anchorx >= size_x)
		return;
	for (size_t i = 0; i < str.length(); i++) {
		if (anchorx >= size_x)
			return;
		setCharAt(anchorx, anchory, str[i]);
		anchorx++;
	}
}

}  // namespace MLib