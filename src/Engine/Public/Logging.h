#pragma once
#include "Engine.h"

#include <fstream>
#include <iostream>
#include <string>

// logging class with minimal dependencies
struct logging
{

	ENGINE_API static std::ofstream logfile;

	ENGINE_API static void init();

};

/// <summary> Loggs the given input to the logfile and to the console.</summary>
#define ENG_LOGLN(ans) logging::logfile << ans << std::endl; std::cout << ans << std::endl

#define ENG_LOG(ans) logging::logfile << ans; std::cout << ans