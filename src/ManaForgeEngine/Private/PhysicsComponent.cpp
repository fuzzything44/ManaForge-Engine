#include "EnginePCH.h"

#include "PhysicsComponent.h"

PhysicsComponent::~PhysicsComponent()
{
	delete body;
}
