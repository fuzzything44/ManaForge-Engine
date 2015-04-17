#pragma once
#include <fstream>
#include <iostream>

struct logging
{

	static std::ofstream logfile;

	static void init()
	{

		logfile.open("log.txt");
		
		if(logfile.bad())
		{
			::exit(-2);
		}
	}

};

/// <summary> Loggs the given input to the logfile and to the console.</summary>
#define ENG_LOG(ans)logging::logfile << ans << std::endl; std::cout << ans << std::endl;

