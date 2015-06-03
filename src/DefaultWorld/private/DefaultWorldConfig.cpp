#include "DefaultWorldConfig.h"

#include <ModuleManager.h>
#include <DefaultWorld.h>


extern "C" DefualtWorld_API void registerModule(ModuleManager& mm)
{
	mm.setCreateWorldFun(
		[](const std::string& name)
	{
		return new DefaultWorld(name);
	});


}

extern "C" DefualtWorld_API float getModuleEngineVersion()
{
	return ENGINE_VERSION;
}
