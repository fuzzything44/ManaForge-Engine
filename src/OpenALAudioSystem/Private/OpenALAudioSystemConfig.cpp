#include "OpenALAudioSystemConfig.h"

#include "OpenALAudioSystem.h"

#include <Runtime.h>

#pragma comment(lib, "OpenAL.lib")
#pragma comment(lib, "alut.lib")

extern "C" OpenALAudioSystem_API void registerModule()
{
	audioManager = AudioManager_t::factory(std::make_tuple(coreManager));
}

extern "C" OpenALAudioSystem_API float getModuleEngineVersion() { return ENGINE_VERSION; }
