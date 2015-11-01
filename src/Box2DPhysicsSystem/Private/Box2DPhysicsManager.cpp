#include "Box2DPhysicsManager.h"

__declspec(dllexport) std::weak_ptr<Box2DPhysicsManager_t> box2DPhysicsManager = {};


extern "C" __declspec(dllexport) void init()
{
	box2DPhysicsManager = Box2DPhysicsManager_t::factory(std::make_tuple(coreManager.lock().get()));
}



