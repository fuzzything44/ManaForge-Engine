#pragma once
#include "DefaultWorldConfig.h"
#include "DefaultWorld.h"
#include <World.h>

#include <map>

struct DefaultWorldLocation : public ActorLocation
{
public:
	DefaultWorldLocation(size_t loc, DefaultWorld& world) : loc(loc), inWorld(world) {};
	virtual ~DefaultWorldLocation() override;
	
private:
	size_t loc;
	DefaultWorld& inWorld;
};
