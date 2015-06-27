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
	virtual void remove() {};
};

class World
{
public:

	virtual ~World() { };

	/// <summary> loads a subworld with given name. </summary>
	/// <param name="subWorldName"> Name of subworld ex. "dungeon1".
	virtual void loadWorld(std::string subWorldName) = 0;

	virtual PlayerController* makePlayerController() = 0;
	virtual Pawn* makePawn() = 0;

	virtual void addActor(Actor* toAdd) = 0;

	virtual void save() = 0;

	virtual void consoleCommand(std::string& command) = 0;

	

};