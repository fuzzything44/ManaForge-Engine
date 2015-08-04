#include "Box2DPhysicsSystemConfig.h"

#include "Box2DPhysicsSystem.h"

#include <ModuleManager.h>

extern "C" Box2DPhysicsSystem_API void registerModule(ModuleManager& manager)
{
	manager.registerClass<Box2DPhysicsSystem>(MODULE_NAME);
	manager.addUpdateCallback([](float delta)
	                          {
		                          return static_cast<Box2DPhysicsSystem*>(Runtime::get().physSystem.get())->update(delta);
		                      });
}

extern "C" Box2DPhysicsSystem_API float getModuleEngineVersion() { return ENGINE_VERSION; }
