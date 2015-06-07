#include "Box2DPhysicsSystemConfig.h"

#include "Box2DPhysicsSystem.h"

#include <ModuleManager.h>

extern "C" Box2DPhysicsSystem_API void registerModule(ModuleManager& manager)
{
	manager.setPhysicsSystem(new Box2DPhysicsSystem());
	manager.addUpdateCallback(std::bind(
		&Box2DPhysicsSystem::update, 
		static_cast<Box2DPhysicsSystem*>(&manager.getPhysicsSystem()), 
			std::placeholders::_1));
}

extern "C" Box2DPhysicsSystem_API float getModuleEngineVersion()
{
	return ENGINE_VERSION;
}
