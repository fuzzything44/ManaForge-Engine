#pragma once
#include "Engine.h"
#include <string>
#include "Chunk.h"
#include "ModuleManager.h"

class WorldModule
{
public:
	/// <summary> Creates and loads a world. </summary>
	/// <param name="name"> The name of the world folder ex. "worlds/mainworld". 
	/// <para> It then loads "worlds/mainworld/main.WORLD", "worlds/mainworld/main.png", and "world/mainworld/main.SAVE" </para> </param>
	WorldModule(std::string folder = "", ModuleManager& mm = ModuleManager() ) {}
	
	/// <summary> loads a subworld with given name. </summary>
	/// <param name="subWorldName"> Name of subworld ex. "dungeon1".
	virtual void loadWorld(std::string subWorldName) = 0;

	/// <summary> Returns the chunk the location falls into. </summary>
	virtual Chunk* getChunk(vec2 actorLocation) = 0;
	virtual int getChunkSize() = 0;
	virtual ~WorldModule() = 0;

};