#pragma once
#include <Engine.h>

#ifdef TestContent_Source
#define TestContent_API __declspec(dllexport)
#else
#define TestContent_API __declspec(dllimport)
#pragma comment(lib, "TestContent.lib")
#endif