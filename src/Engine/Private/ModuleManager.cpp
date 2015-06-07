#include "EnginePCH.h"

#include "ModuleManager.h"

#include "Helper.h"

#include <map>

ModuleManager::ModuleManager()
	: renderer(nullptr)
{

}

ModuleManager::~ModuleManager()
{
	newWorldFunction.~function();


	for (auto& elem : contentSpawnMethods)
	{
		elem.second.~function();
	}

	for (auto& elem : initCallbacks)
	{
		elem.~function();
	}

	for (auto& elem : updateCallbacks)
	{
		elem.~function();
	}

	delete renderer;
}

Renderer& ModuleManager::getRenderer()
{
	check(renderer);

    return *renderer;
	
}

AudioSystem& ModuleManager::getAudioSystem()
{
	check(audioSystem);

	return *audioSystem;
}


void ModuleManager::setRenderer(Renderer* newRenderer)
{
	check(newRenderer);

	renderer = newRenderer;
	
}

void ModuleManager::setAudioSystem(AudioSystem* newAudioSystem)
{
	check(newAudioSystem);

	audioSystem = newAudioSystem;
}

void ModuleManager::loadModule(const std::string& filename)
{

	if (loadedModules.find(filename) == loadedModules.end())
	{
		loadedModules.insert(std::map<std::string, Module>::value_type(filename, Module(filename))).first->second.registerModule(*this);
	}
}

void ModuleManager::setCreateWorldFun(const newWorldFun& createWorldFun)
{
	
	// copy to the class's version
	newWorldFunction =  createWorldFun;
}

World* ModuleManager::newWorld(std::string path)
{
	if (newWorldFunction._Empty())
	{
		FATAL_ERR("Failed to create world -- the function was empty");
	}

	return newWorldFunction(path);
}

void ModuleManager::AddContentModule(contentModuleSpawnFun fun, const std::string& moduleName)
{
	// add the function to the spawn method map
	contentSpawnMethods.insert(std::pair<std::string, contentModuleSpawnFun>(moduleName, fun));
}



void ModuleManager::addInitCallback(const std::function<void()>& function)
{
	initCallbacks.push_back(function);
}

void ModuleManager::addUpdateCallback(const std::function<bool()>& function)
{
	updateCallbacks.push_back(function);
}

std::list<std::function<void()> >& ModuleManager::getInitCallbacks()
{
	return initCallbacks;
}

std::list<std::function<bool()> >& ModuleManager::getUpdateCallbacks()
{
	return updateCallbacks;
}