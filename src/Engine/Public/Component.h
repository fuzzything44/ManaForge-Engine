#pragma once

#include "Engine.h"

class Actor;

class Component
{
public:
	ENGINE_API explicit Component(Actor* owner = nullptr);
	ENGINE_API virtual ~Component();

	ENGINE_API inline Actor* getOwner();

protected:
	Actor* owner;

};

inline Actor* Component::getOwner()
{
	return owner;
}