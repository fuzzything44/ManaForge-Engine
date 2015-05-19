#pragma once

#include "Engine.h"

class Actor;

class Component
{
public:
	explicit ENGINE_API Component(Actor* owner = nullptr);
	ENGINE_API virtual ~Component();
protected:
	Actor* owner;

};