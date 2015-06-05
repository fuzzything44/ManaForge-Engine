#include "TestContentConfig.h"
#include "Gate.h"

#include <ModuleManager.h>


void* spawnClass(std::string qualifier)
{
	if (qualifier == "Gate")
	{
		return new Gate();
	}

	return nullptr;
}

extern "C" TestContent_API void registerModule(ModuleManager& mm)
{
	mm.AddContentModule(spawnClass, TestContent_ModuleName);
}

extern "C" TestContent_API float getModuleEngineVersion()
{
	return ENGINE_VERSION;
}
