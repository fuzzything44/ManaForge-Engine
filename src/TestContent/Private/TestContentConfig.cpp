#include "TestContentConfig.h"
#include "Gate.h"

#include <ModuleManager.h>

#include <map>

typedef std::map<std::string, std::function<void*() > > spawnFuncMapType;

const/*expr*/ spawnFuncMapType spawnFuncs = 
{
	spawnFuncMapType::value_type("Gate", []() 
	{
		return new Gate(); 
	}),


};

void* spawnClass(std::string qualifier)
{
	auto iter = spawnFuncs.find(qualifier);

	if (iter != spawnFuncs.end())
	{
		return iter->second();
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
