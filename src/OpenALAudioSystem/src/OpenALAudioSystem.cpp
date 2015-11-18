#include "OpenALAudioSystem.h"

#include <ModuleHandler.h>

std::weak_ptr<OpenALAudioManager_t> openALAudioManager = {};

extern "C" DLLEXPORT void init(ModuleHandler&)
{
	openALAudioManager = OpenALAudioManager_t::factory(OpenALAudioManager_t::MyBasePtrStorage_t(coreManager.lock().get()));
}



