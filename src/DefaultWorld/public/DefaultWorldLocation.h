#pragma once
#include "DefaultWorldConfig.h"
#include "DefaultWorld.h"
#include <World.h>

#include <deque>

struct DefaultWorldLocation : public ActorLocation
{
public:
	DefaultWorldLocation(size_t loc, DefaultWorld& world)
		: loc(loc)
		, inWorld(world){};
	virtual ~DefaultWorldLocation() override;

private:
	std::deque<Actor*>::size_type loc;
	DefaultWorld& inWorld;
};
