#include "colorBuffer.h"
#include "misc.h"

namespace MLib
{
ColorBuffer::ColorBuffer()
{
}

ColorBuffer::~ColorBuffer()
{
}

constexpr uint32_t ColorBuffer::BLANK;
constexpr uint32_t ColorBuffer::EMPTY;

void ColorBuffer::resize(long x, long y)
{
	if (x < 0 || y < 0)
		return;
	size_x = x;
	size_y = y;
	frame.resize(x * y);
	nxframe.resize(x * y);
	clear();
}

void ColorBuffer::clear()
{
	frame.assign(size_x * size_y, BLANK);
	nxframe.assign(size_x * size_y, BLANK);
}

void ColorBuffer::getSize(vector2_t<long>& target) const
{
	target.x = size_x;
	target.y = size_y;
}

bool ColorBuffer::set(long x, long y, uint32_t pack)
{
	if (x < 0 || y < 0 || x >= size_x || y >= size_y)
		return false;
	long pos = y * size_x + x;
	if (nxframe[pos] != pack) {
		nxframe[pos] = pack;
		needUpdate = true;
	}
	return true;
}

bool ColorBuffer::set(long x, long y, uint8_t colorB, uint8_t colorF, char c)
{
	return set(x, y, makePack(colorB, colorF, c));
}

void ColorBuffer::draw()
{
	if (!needUpdate)
		return;
	uint8_t bgc = Color::Black;
	uint8_t fgc = Color::BrightWhite;
	fprintf(stdout, "%c[0m", 0x1B);
	setConsoleBgColor(bgc);
	setConsoleFgColor(fgc);
	for (long y = 0; y < size_y; y++) {
		bool last = false;
		for (long x = 0; x < size_x; x++) {
			long pos = y * size_x + x;
			uint32_t pack = nxframe[pos];
			if (frame[pos] == pack) {
				last = false;
				continue;
			}
			uint8_t tbgc = packGetBColor(pack);
			uint8_t tfgc = packGetFColor(pack);
			char c = packGetChar(pack);
			if (tbgc != bgc) {
				bgc = tbgc;
				setConsoleBgColor(bgc);
			}
			if (tfgc != fgc) {
				fgc = tfgc;
				setConsoleFgColor(fgc);
			}
			if (!last) {
				setConsolePos(x, y);
			}
			putchar(c);
			frame[pos] = pack;
			last = true;
		}
	}
	needUpdate = false;
	setConsolePos(0, 0);
	fprintf(stdout, "%c[0m", 0x1B);
	fflush(stdout);
}

void ColorBuffer::redraw()
{
	uint8_t bgc = Color::Black;
	uint8_t fgc = Color::BrightWhite;
	fprintf(stdout, "%c[0m", 0x1B);
	setConsoleBgColor(bgc);
	setConsoleFgColor(fgc);
	setConsolePos(0, 0);
	for (long y = 0; y < size_y; y++) {
		for (long x = 0; x < size_x; x++) {
			long pos = y * size_x + x;
			uint32_t pack = nxframe[pos];
			uint8_t tbgc = packGetBColor(pack);
			uint8_t tfgc = packGetFColor(pack);
			char c = packGetChar(pack);
			if (tbgc != bgc) {
				bgc = tbgc;
				setConsoleBgColor(bgc);
			}
			if (tfgc != fgc) {
				fgc = tfgc;
				setConsoleFgColor(fgc);
			}
			putchar(c);
			frame[pos] = pack;
		}
		putchar('\n');
	}
	needUpdate = false;
	fprintf(stdout, "%c[0m", 0x1B);
	fflush(stdout);
}

}  // namespace MLib