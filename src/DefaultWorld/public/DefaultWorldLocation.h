#pragma once
#include "DefaultWorldConfig.h"
#include "DefaultWorld.h"
#include <World.h>

#include <map>

struct DefaultWorldLocation : public ActorLocation
{
public:
	DefaultWorldLocation(DefaultWorld::iter_type loc, DefaultWorld& world) : location(loc), inWorld(world) {};
	virtual ~DefaultWorldLocation() override;
	
private:
	DefaultWorld::iter_type location;
	DefaultWorld& inWorld;
};
