#pragma once
#include <fstream>
#include <iostream>

namespace logging
{

	static std::ofstream logfile = std::ofstream();

	inline void init()
	{

		logfile.open("log.txt");
	}

};

/// <summary> Loggs the given input to the logfile and to the console.</summary>
#define ENG_LOG(ans)logging::logfile << ans << std::endl; std::cout << ans << std::endl;

