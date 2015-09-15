#pragma once

#include "Engine.h"

class Actor;

class Component
{
public:
	inline explicit Component(Actor& owner);
	ENGINE_API virtual ~Component();

	inline Actor& getOwner();
	inline const Actor& getOwner() const;

protected:
	Actor& owner;

private:
	std::deque<Component*>::size_type location;
};

#include "Actor.h"

inline Component::Component(Actor& owner)
	: owner(owner)
{
	owner.components.push_back(this);
	location = owner.components.size() - 1;
}

inline Actor& Component::getOwner() { return owner; }

inline const Actor& Component::getOwner() const { return owner; }