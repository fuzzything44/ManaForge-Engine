#include "OpenALAudioSystemConfig.h"

#include "OpenALAudioSystem.h"

#include <Runtime.h>
#include <ModuleManager.h>

#pragma comment(lib, "OpenAL.lib")
#pragma comment(lib, "alut.lib")

extern "C" OpenALAudioSystem_API void registerModule(ModuleManager& manager)
{
	manager.registerClass<OpenALAudioSystem>(MODULE_NAME);
	manager.addUpdateCallback([&manager](float delta)
		{
			return static_cast<OpenALAudioSystem*>(Runtime::get().audioSystem.get())->update();

		});
}

extern "C" OpenALAudioSystem_API float getModuleEngineVersion() { return ENGINE_VERSION; }
