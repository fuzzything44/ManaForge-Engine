#pragma once
#include "Engine.h"

#include <string>
#include <map>

class Actor;
class ModuleManager;

class World
{
protected:

public:

	// global typedef for the mapID type
	typedef uint64 map_ID_t;


	/// <summary> Creates and loads a world. </summary>
	/// <param name="name"> The name of the world folder ex. "worlds/mainworld". 
	/// <para> It then loads "worlds/mainworld/main.WORLD", "worlds/mainworld/main.png", and "world/mainworld/main.SAVE" </para> </param>
	explicit World(ModuleManager& mm, std::string folder = "") { }
	
	World() { }

	/// <summary> loads a subworld with given name. </summary>
	/// <param name="subWorldName"> Name of subworld ex. "dungeon1".
	virtual void loadWorld(std::string subWorldName) = 0;

	virtual void addActor(Actor& toAdd) = 0;

	virtual void save() = 0;

	virtual void consoleCommand(std::string& command) = 0;

	virtual ~World() { };
};