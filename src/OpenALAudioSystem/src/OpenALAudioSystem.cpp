#include "OpenALAudioSystem.h"

#include <ModuleHandler.h>

OpenALAudioManager_t* openALAudioManager = nullptr;

extern "C" DLLEXPORT void init(ModuleHandler&)
{
	openALAudioManager = OpenALAudioManager_t::factory(OpenALAudioManager_t::MyBasePtrStorage_t(coreManager));
}

extern"C" DLLEXPORT void cleanup(ModuleHandler& handler)
{
	delete openALAudioManager;
	openALAudioManager = nullptr;
}

