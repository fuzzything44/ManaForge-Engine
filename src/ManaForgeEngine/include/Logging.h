#pragma once
#include "Engine.h"

#include <fstream>
#include <iostream>

namespace logdetail
{
	MF_API extern std::ofstream outFile;
}


#define MFLOG(msg) \
	(::std::cout << msg) && (::logdetail::outFile << msg)