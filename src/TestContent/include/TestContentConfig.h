#pragma once
#include <Engine.h>

#ifdef TestContent_Source
#define TestContent_API DLLEXPORT
#else
#define TestContent_API DLLIMPORT
#pragma comment(lib, "TestContent.lib")
#endif
