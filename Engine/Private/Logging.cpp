#include "stdafx.h"
#include "Logging.h"

std::fstream logging::logfile = std::fstream();

void logging::init()
{
	logfile.open("../log.txt", std::fstream::out);
}
