#include "Box2DPhysicsManager.h"

std::shared_ptr<Box2DPhysicsManager_t> box2DPhysicsManager = Box2DPhysicsManager_t::factory(std::make_tuple(coreManager));
