#include "DefaultWorldConfig.h"

#include <ModuleManager.h>
#include <DefaultWorld.h>


extern "C" DefualtWorld_API void registerModule(ModuleManager& mm)
{
	mm.registerClass<DefaultWorld>(MODULE_NAME);
}

extern "C" DefualtWorld_API float getModuleEngineVersion()
{
	return ENGINE_VERSION;
}
