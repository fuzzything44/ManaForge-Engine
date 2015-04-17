#include "Engine.h"
#include "ModuleManager.h"
#include "Helper.h"

ModuleManager* ModuleManager::currentMM = nullptr;

ModuleManager::ModuleManager()
	: renderer(nullptr)
{
}

Renderer* ModuleManager::getRenderer()
{


	if (renderer)
		return renderer;

	return nullptr;
	
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

		ENG_LOG(renderer);
	}
}

void ModuleManager::addWorld(World* createWorld(ModuleManager&, std::string))
{
	STACK
		// copy to the class's version
		this->createWorld = createWorld;
}

ENGINE_API ModuleManager* ModuleManager::get()
{
	assert(currentMM);

	return currentMM;
}