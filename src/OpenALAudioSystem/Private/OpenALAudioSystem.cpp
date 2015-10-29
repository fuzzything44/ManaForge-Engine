#include "OpenALAudioSystem.h"


std::shared_ptr<OpenALAudioManager_t> audioManager;

extern "C" __declspec(dllexport) void init()
{
	audioManager = OpenALAudioManager_t::factory(std::make_tuple(coreManager));
}



