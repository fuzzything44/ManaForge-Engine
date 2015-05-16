#pragma once

#define TestContent_ModuleName "TestContent"

#ifdef TestContent_Source
#	define TestContent_API __declspec(dllexport)
#else
#	define TestContent_API __declspec(dllimport)
#	pragma comment(lib, "TestContent.lib")
#endif