#include "TestContentConfig.h"
#include "Gate.h"
#include "Pew.h"
#include <ModuleManager.h>

#include <map>


extern "C" TestContent_API void registerModule(ModuleManager& mm)
{
	mm.registerClass<Gate>(MODULE_NAME);
	mm.registerClass<Pew>(MODULE_NAME);
}

extern "C" TestContent_API float getModuleEngineVersion()
{
	return ENGINE_VERSION;
}
