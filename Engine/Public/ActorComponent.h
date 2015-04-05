#pragma once 

#include "Engine.h"
#include "Actor.h"

class ActorComponent
{
public:
	ActorComponent(Actor* owner);
	Actor* getOwner();
	~ActorComponent();
private:
	Actor* owner;
};