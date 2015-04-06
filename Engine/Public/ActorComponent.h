#pragma once 

#include "Engine.h"
#include "Actor.h"

class ActorComponent
{
public:
	explicit ActorComponent(Actor* owner);
	Actor* getOwner();
	virtual ~ActorComponent();
private:
	Actor* owner;
};