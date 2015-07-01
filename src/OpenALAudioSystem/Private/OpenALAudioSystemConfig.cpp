#include "OpenALAudioSystemConfig.h"

#include "OpenALAudioSystem.h"

#include <ModuleManager.h>

extern "C" OpenALAudioSystem_API void registerModule(ModuleManager& manager)
{
	manager.setAudioSystem(new OpenALAudioSystem());
	manager.addUpdateCallback([&manager](float delta)
	{
		return static_cast<OpenALAudioSystem*>(&(manager.getAudioSystem()))->update();

	});
}

extern "C" OpenALAudioSystem_API float getModuleEngineVersion()
{
	return ENGINE_VERSION;
}
