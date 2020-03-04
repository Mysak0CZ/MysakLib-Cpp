#include "logger.h"

namespace MLib
{
Log::Log(unsigned type, bool internal)
{
	level = type;
	char buf[128];
	time_t rawtime;
	struct tm* info;
	time(&rawtime);
	info = localtime(&rawtime);

	std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", info);
	if (level <= MysakLib::INSTANCE.loglevel) {
		MysakLib::INSTANCE.logfile << '[' << buf << ']' << (internal ? " I " : "   ");
		writeTypeString(type);
	}
}

Log::~Log()
{
	if (level <= MysakLib::INSTANCE.loglevel) {
		MysakLib::INSTANCE.logfile << std::endl;
		MysakLib::INSTANCE.logfile.flush();
	}
}

}  // namespace MLib
