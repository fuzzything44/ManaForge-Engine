#include "Logging.h"

std::ofstream logging::logfile = std::ofstream();

ENGINE_API void logging::init()
{

	logfile.open("log.txt");

	if (logfile.bad())
	{
		::exit(-2);
	}
}