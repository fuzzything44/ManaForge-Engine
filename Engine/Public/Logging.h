#pragma once
#include <fstream>
#include <iostream>

namespace logging
{

	std::ofstream logfile;

	static void init()
	{
		logfile.open("log.txt");
	}

};

/// <summary> Loggs the given input to the logfile and to the console.</summary>
#define ENG_LOG(ans)logging::logfile << ans << std::endl; std::cout << ans << std::endl;

