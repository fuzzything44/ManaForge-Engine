#pragma once

#include "Engine.h"

class Actor;

class Component
{
public:
	ENGINE_API explicit Component(Actor* owner = nullptr);
	ENGINE_API virtual ~Component();
protected:
	Actor* owner;

};