#pragma once

#include "Engine.h"

class Actor;

class Component
{
public:
	explicit ENGINE_API Component(Actor* owner = nullptr);
private:
	Actor* owner;

};

#include "Actor.h"
