#include "Box2DPhysicsManager.h"

__declspec(dllexport) std::shared_ptr<Box2DPhysicsManager_t> box2DPhysicsManager{ nullptr };


extern "C" __declspec(dllexport) void init()
{
	box2DPhysicsManager = Box2DPhysicsManager_t::factory(std::make_tuple(coreManager));
}



