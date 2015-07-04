#pragma once
#include "Engine.h"

#include <string>

class Actor;
class ModuleManager;
class Pawn;
class PlayerController;

// Struct used to find actor in the world.
struct ActorLocation
{
	virtual ~ActorLocation(){ };
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

	virtual void registerTickingActor(Actor& toAdd) = 0;

};