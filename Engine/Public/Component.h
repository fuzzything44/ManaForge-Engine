#pragma once

#include "Engine.h"
#include "Actor.h"

class ENGINE_API Component
{
public:
	explicit Component(Actor* owner);
private:
	Actor* owner;

};
