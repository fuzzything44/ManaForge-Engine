#pragma once 

#include "Engine.h"
#include "Module.h"
#include "Renderer.h"

#include <map>
#include <vector>

class ModuleManager
{


public:

	/// <summary> Gets the renderer./</summary>
	///
	/// <returns> if it fails, returns null and throws an exception, else the renderer.</returns>
	Renderer* getRenderer()
	{
		if (renderer)
		{
			return renderer;
		}
		else
		{
			throw std::runtime_error("Renderer is null, please select renderer");
			return NULL;
		}
	}

	/// <summary> Loads a module.</summary>
	///
	/// <param name="filename"> Filename of the module.</param>
	void LoadModule(const std::string& filename)
	{
		if (loadedModules.find(filename) == loadedModules.end())
		{
			loadedModules.insert(std::map<std::string, Module>::value_type(filename, Module(filename))).first->second.registerModule(*this);
		}
	}

	/// <summary> Adds a renderer.</summary>
	///
	/// <param name="newRenderer"> If non-null, the new renderer.</param>
	void AddRenderer(Renderer* newRenderer)
	{
		AvaliableRenderers.push_back(newRenderer);

		if (!newRenderer)
		{
			renderer = newRenderer;
		}
	}

private:


	std::map<std::string, Module> loadedModules;

	std::vector<Renderer*> AvaliableRenderers;
	Renderer* renderer;
};
