#pragma once
#ifdef _MSC_VER
#	ifdef DefaultWorldSource
#		define DefaultWorldPlugin_API __declspec(dllexport)
#	else
#		define DefaultWorldPlugin_API __declspec(dllimport)
#	endif
#endif