#include "stdafx.h"
#include "ActorComponent.h"

ActorComponent::ActorComponent(Actor* owner) 
	: owner(owner)
{
	
}
Actor* ActorComponent::getOwner() {
	return owner;
}

ActorComponent::~ActorComponent()
{
}