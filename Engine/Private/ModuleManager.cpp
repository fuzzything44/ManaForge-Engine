#include "Engine.h"
#include "ModuleManager.h"
#include "Helper.h"

ModuleManager* ModuleManager::currentMM = nullptr;

ModuleManager::ModuleManager()
	: renderer(nullptr)
{
	currentMM = this;
}

Renderer& ModuleManager::getRenderer()
{


       check(renderer);

       return *renderer;
	
}

void ModuleManager::loadModule(const std::string& filename)
{
	STACK
		if (loadedModules.find(filename) == loadedModules.end())
		{
			loadedModules.insert(std::map<std::string, Module>::value_type(filename, Module(filename))).first->second.registerModule(*this);
		}
}

void ModuleManager::addRenderer(Renderer* newRenderer)
{
	STACK
		avaliableRenderers.push_back(newRenderer);

	if (!renderer)
	{
		renderer = newRenderer;
	}
}

void ModuleManager::addWorld(World* createWorld(ModuleManager&, std::string))
{
	STACK
		// copy to the class's version
		this->createWorld = createWorld;
}

ModuleManager& ModuleManager::get()
{
	check(currentMM);

	return *currentMM;
}
