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
	
	



	/// <summary> Creates and loads a world. </summary>
	/// <param name="name"> The name of the world folder ex. "worlds/mainworld". 
	/// <para> It then loads "worlds/mainworld/main.WORLD", "worlds/mainworld/main.png", and "world/mainworld/main.SAVE" </para> </param>
	explicit World(ModuleManager& mm, std::string folder = "") { }
	
	World() { }

	/// <summary> loads a subworld with given name. </summary>
	/// <param name="subWorldName"> Name of subworld ex. "dungeon1".
	virtual void loadWorld(std::string subWorldName) = 0;

	virtual PlayerController* makePlayerController() = 0;
	virtual Pawn* makePawn() = 0;

	virtual void addActor(Actor* toAdd) = 0;

	virtual void save() = 0;

	virtual void consoleCommand(std::string& command) = 0;

	virtual ~World() { };
};