#include "stdafx.h"
#include "Logging.h"

std::fstream logging::logfile = std::fstream();

void logging::init()
{STACK
	logfile.open("../log.txt", std::fstream::out);
}
