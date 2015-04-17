#pragma once 

#include "Engine.h"
#include "Module.h"
#include "Renderer.h"
#include "World.h"

#include <map>
#include <vector>

class World;
class Renderer;

class ModuleManager
{


public:

	/// <summary> Default constructor.</summary>
	ModuleManager() { }

	/// <summary> Gets the renderer./</summary>
	///
	/// <returns> if it fails, returns null and throws an exception, else the renderer.</returns>
	Renderer* getRenderer()
	{STACK
		if (renderer)
		{
			return renderer;
		}
		throw std::runtime_error("Renderer is null, please select renderer");
		return nullptr;
	}

	

	/// <summary> Loads a module.</summary>
	///
	/// <param name="filename"> Filename of the module.</param>
	void loadModule(const std::string& filename)
	{STACK
		if (loadedModules.find(filename) == loadedModules.end())
		{
			loadedModules.insert(std::map<std::string, Module>::value_type(filename, Module(filename))).first->second.registerModule(*this);
		}
	}

	/// <summary> Adds a renderer.</summary>
	///
	/// <param name="newRenderer"> If non-null, the new renderer.</param>
	void addRenderer(Renderer* newRenderer)
	{STACK
		avaliableRenderers.push_back(newRenderer);

		if (!newRenderer)
		{
			renderer = newRenderer;
		}
	}

	/// <summary> Adds a world.</summary>
	///
	/// <param name="createWorld()"> Function that creates a world </param>
	void addWorld(World* createWorld(ModuleManager&, std::string))
	{STACK
		// copy to the class's version
		this->createWorld = createWorld;
	}

	ENGINE_API static ModuleManager& get()
	{
		return *currentMM;
	}

private:

	static ModuleManager* currentMM;

	// function to createWorld
	World*(*createWorld)(ModuleManager&, std::string);

	std::map<std::string, Module> loadedModules;

	std::vector<Renderer*> avaliableRenderers;
	Renderer* renderer;
};
