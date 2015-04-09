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
	void LoadModule(const std::string& filename)
	{STACK
		if (loadedModules.find(filename) == loadedModules.end())
		{
			loadedModules.insert(std::map<std::string, Module>::value_type(filename, Module(filename))).first->second.registerModule(*this);
		}
	}

	/// <summary> Adds a renderer.</summary>
	///
	/// <param name="newRenderer"> If non-null, the new renderer.</param>
	void AddRenderer(Renderer* newRenderer)
	{STACK
		AvaliableRenderers.push_back(newRenderer);

		if (!newRenderer)
		{
			renderer = newRenderer;
		}
	}

	/// <summary> Adds a world.</summary>
	///
	/// <param name="createWorld()"> Function that creates a world </param>
	void AddWorld(World* createWorld(ModuleManager&, std::string))
	{STACK
		// copy to the class's version
		this->createWorld = createWorld;
	}

private:

	// function to createWorld
	World*(*createWorld)(ModuleManager&, std::string);

	std::map<std::string, Module> loadedModules;

	std::vector<Renderer*> AvaliableRenderers;
	Renderer* renderer;
};
