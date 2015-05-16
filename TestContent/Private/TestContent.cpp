#include "TestContentConfig.h"
#include "Gate.h"

#include <ModuleManager.h>


Actor* spawnActor(std::string qualifier, const Transform& trans)
{
	if (qualifier == "Gate")
	{
		return new Gate(trans);
	}

	return nullptr;
}

extern "C" TestContent_API void registerModule(ModuleManager& mm)
{
	mm.AddContentModule(spawnActor, TestContent_ModuleName);
}

extern "C" TestContent_API float getModuleEngineVersion()
{
	return ENGINE_VERSION;
}
