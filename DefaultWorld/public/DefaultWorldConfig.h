#pragma once

#define DefaultWorld_ModuleName "DefaultWorld"

#include <boost/preprocessor.hpp>

#ifdef _MSC_VER
#	ifdef DefaultWorldSource
#		define DefualtWorld_API __declspec(dllexport)
#	else
#		define DefaultWorldPlugin_API __declspec(dllimport)
#		pragma comment(lib, BOOST_PP_CAT(DefaultWorld_ModuleName, ".lib")
#	endif
#endif