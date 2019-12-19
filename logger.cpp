#include "logger.h"

namespace MLib
{
Logger Logger::LOGGER;

Logger::Logger()
{
	outfile.open(M_LOGFILE_NAME, std::ofstream::out | std::ofstream::trunc);
	Log(LOG_verbose, true) << "Logger initialized";
}

Logger::~Logger()
{
	outfile.close();
}

Log::Log(unsigned type, bool internal)
{
	level = type;
	char buf[128];
	time_t rawtime;
	struct tm* info;
	time(&rawtime);
	info = localtime(&rawtime);

	std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", info);
	if (level <= Logger::LOGGER.level) {
		Logger::LOGGER << '[' << buf << "]" << (internal ? " I " : "   ");
		writeTypeString(type);
	}
}

Log::~Log()
{
	if (level <= Logger::LOGGER.level)
		Logger::LOGGER << '\n';
}

}  // namespace MLib
