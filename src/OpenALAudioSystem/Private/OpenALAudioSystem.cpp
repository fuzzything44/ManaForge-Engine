#include "OpenALAudioSystem.h"


std::weak_ptr<OpenALAudioManager_t> openALAudioManager = {};

extern "C" __declspec(dllexport) void init()
{
	openALAudioManager = OpenALAudioManager_t::factory(OpenALAudioManager_t::MyBasePtrStorage_t(coreManager.lock().get()));
}



