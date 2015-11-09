#ifdef OpenALAudioSystem_Source
#	define OpenALAudioSystem_API __declspec(dllexport)
#else
#	define OpenALAudioSystem_API __declspec(dllimport)
#	pragma comment(lib, "OpenALAudioSystem.lib")
#endif