#include "EnginePCH.h"

#include "Component.h"
#include "Actor.h"

Component::Component(Actor* owner)
	: owner(owner)
{
		owner->components.push_back(this);
}

Component::~Component()
{
}