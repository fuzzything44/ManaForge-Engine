#pragma once
#include "Box2DPhysicsSystemConfig.h"

#include "CBox2DCollision.h"

#include <CoreManager.h>


class PhysicsComponent;
class Box2DActorTransformController;

using Box2DPhysicsManager_t = 
	Manager
	<
		boost::mpl::vector1<CBox2DCollision>
		, boost::mpl::vector0<>
		, boost::mpl::vector1<CoreManager_t>
	>;

template<>
struct ManagerData<Box2DPhysicsManager_t>
{
	ManagerData()
		:world{ b2Vec2{ 0.f, 0.f } }
	{

	}

	b2World world;
};

template<>
void initManager<Box2DPhysicsManager_t>(Box2DPhysicsManager_t& manager)
{
	 
}



template<>
void updateManager<Box2DPhysicsManager_t>(Box2DPhysicsManager_t& manager)
{
	auto&& world = manager.getManagerData().world;
	world.Step(0/*TODO: implemnt delta times*/, 8, 3); // step once

}


Box2DPhysicsSystem_API extern std::weak_ptr<Box2DPhysicsManager_t> box2DPhysicsManager;
