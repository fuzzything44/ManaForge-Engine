#pragma once 

#include "Engine.h"
#include "Module.h"
#include "Renderer.h"
#include "World.h"

#include <map>
#include <vector>
#include <functional>

class World;
class Renderer;

class ModuleManager
{
public:

	ENGINE_API ~ModuleManager();

	/// <summary> Default constructor.</summary>
	ENGINE_API ModuleManager();

	/// <summary> Gets the renderer./</summary>
	///
	/// <returns> if it fails, returns null and throws an exception, else the renderer.</returns>
	ENGINE_API Renderer& getRenderer();
		
	/// <summary> Loads a module.</summary>
	///
	/// <param name="filename"> Filename of the module.</param>
	ENGINE_API 	void loadModule(const std::string& filename);

	/// <summary> Adds a renderer.</summary>
	///
	/// <param name="newRenderer"> If non-null, the new renderer.</param>
	ENGINE_API void addRenderer(Renderer* newRenderer);

	/// <summary> Adds a world.</summary>
	///
	/// <param name="createWorld()"> Function that creates a world </param>
	ENGINE_API void setWorld(std::function<World*(std::string)> createWorld);

	ENGINE_API World* newWorld(std::string path);

private:

	// function to createWorld
	std::function<World*(std::string)>* createWorld;

	std::map<std::string, Module> loadedModules;

	std::vector<Renderer*> avaliableRenderers;
	Renderer* renderer;
};