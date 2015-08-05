#pragma once

#include <Engine.h>

#ifdef _MSC_VER
#ifdef DefaultWorld_Source
#define DefualtWorld_API __declspec(dllexport)
#else
#define DefaultWorldPlugin_API __declspec(dllimport)
#pragma comment(lib, BOOST_PP_CAT(DefaultWorld_ModuleName, ".lib")
#endif
#endif