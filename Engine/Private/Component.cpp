#include "Component.h"

Component::Component(Actor* owner)
	: owner(owner)
{
		owner->components.push_back(this);
}