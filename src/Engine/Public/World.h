#pragma once
#include "Engine.h"

#include <string>

class Actor;
class ModuleManager;
class Pawn;
class PlayerController;

// Struct used to find actor in the world. remove removes it from the world, getActor returns the actor or nullptr if not set.
struct ActorLocation
{
	virtual Actor* getActor() { return nullptr; };
	virtual void remove() = 0;
};

class World
{
public:

	virtual ~World() { };

	virtual void init(const std::string& name) = 0;

	virtual void saveWorld() = 0;

	virtual PlayerController* makePlayerController() = 0;
	virtual Pawn* makePawn() = 0;

	virtual ActorLocation* addActor(Actor* toAdd) = 0;
	virtual void removeActor(Actor* toRemove) = 0;	

};