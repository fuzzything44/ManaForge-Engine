#include "OpenALAudioSystem.h"


std::shared_ptr<OpenALAudioManager_t> openALAudioManager;

extern "C" __declspec(dllexport) void init()
{
	openALAudioManager = OpenALAudioManager_t::factory(std::make_tuple(coreManager));
}



