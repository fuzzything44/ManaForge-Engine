#include "Component.h"

Component::Component(Actor* owner)
	: owner(owner)
{
	STACK
		owner->components.push_back(this);
}