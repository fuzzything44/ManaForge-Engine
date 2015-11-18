#include <Engine.h>

#ifdef OpenALAudioSystem_Source
#	define OpenALAudioSystem_API DLLEXPORT
#else
#	define OpenALAudioSystem_API DLLIMPORT
#	pragma comment(lib, "OpenALAudioSystem.lib")
#endif
