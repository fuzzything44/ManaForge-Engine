#pragma once
#include <fstream>
#include <iostream>
#include <Engine.h>

struct logging
{

	ENGINE_API static std::ofstream logfile;

	ENGINE_API static void init();

};

/// <summary> Loggs the given input to the logfile and to the console.</summary>
#define ENG_LOG(ans) logging::logfile << ans << std::endl; std::cout << ans << std::endl;

