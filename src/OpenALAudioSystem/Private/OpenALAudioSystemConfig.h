#pragma once

#include <Engine.h>

#ifdef OpenALAudioSystem_Source
#	define OpenALAudioSystem_API __declspec(dllexport)
#	if defined(_M_X64) || defined(__amd64__)
#		ifdef _DEBUG
#			pragma comment(lib, "alut64d.lib")
#		else
#			pragma comment(lib, "alut64.lib")
#		endif
#	endif
#	pragma comment(lib, "OpenAL32.lib")
#else
#	define OpenALAudioSystem_API __declspec(dllimport)
#	pragma comment(lib, "OpenALAudioSystem.lib")
#endif
