#pragma once

#include "Engine.h"
#include "Actor.h"

class Component
{
public:
	explicit ENGINE_API Component(Actor* owner = nullptr);
private:
	Actor* owner;

};
