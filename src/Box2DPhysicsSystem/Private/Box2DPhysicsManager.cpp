#include "Box2DPhysicsManager.h"

#include <ModuleHandler.h>

__declspec(dllexport) std::weak_ptr<Box2DPhysicsManager_t> box2DPhysicsManager = {};


extern "C" __declspec(dllexport) void init(ModuleHandler&)
{
	box2DPhysicsManager = Box2DPhysicsManager_t::factory(std::make_tuple(coreManager.lock().get()));
}



