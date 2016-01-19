#pragma once
#include "Box2DPhysicsSystemConfig.h"

#include "CBox2DCollision.h"

#include <CoreManager.h>


class PhysicsComponent;
class Box2DActorTransformController;

using Box2DPhysicsManager_t = 
	Manager
	<
		decltype(make_type_tuple<CBox2DCollision>)
		,decltype(make_type_tuple<CoreManager_t>)	
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




//template<>
//void updateManager<Box2DPhysicsManager_t>(Box2DPhysicsManager_t& manager)
//{
//	auto&& world = manager.getManagerData().world;
//	world.Step(0/*TODO: implemnt delta times*/, 8, 3); // step once
//
//}


Box2DPhysicsSystem_API extern Box2DPhysicsManager_t* box2DPhysicsManager;
