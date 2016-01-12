#include "OpenALAudioSystem.h"

#include <ModuleHandler.h>

OpenALAudioManager_t* openALAudioManager = nullptr;

extern "C" DLLEXPORT void init_module(ModuleHandler&)
{
	
	openALAudioManager = new OpenALAudioManager_t(boost::hana::make_tuple(coreManager));
}

extern"C" DLLEXPORT void cleanup_module(ModuleHandler& handler)
{
	delete openALAudioManager;
	openALAudioManager = nullptr;
}

