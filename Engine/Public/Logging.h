#pragma once
#include "Engine.h"
#include <fstream>
#include <sstream>


class logging
{
public:
	static ENGINE_API void init();

	static ENGINE_API std::fstream logfile;

private:

};


#define ENG_LOG(ans)logging::logfile << ans;std::cout << ans;			