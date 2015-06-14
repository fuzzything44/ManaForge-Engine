#include "EnginePCH.h"

#include "ModuleManager.h"

#include "Helper.h"
#include "AudioSystem.h"
#include "Module.h"
#include "PhysicsSystem.h"
#include "AudioSystem.h"
#include "Renderer.h"

#include <map>

ModuleManager::ModuleManager()
	: renderer(nullptr)
{

}

ModuleManager::~ModuleManager()
{
	// explicitly call the destructors -- we get an error otherwise
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
	delete audioSystem;
	delete physicsSystem;
}

Renderer& ModuleManager::getRenderer()
{ check(renderer); return *renderer; }

AudioSystem& ModuleManager::getAudioSystem()
{ check(audioSystem); return *audioSystem; }

PhysicsSystem& ModuleManager::getPhysicsSystem()
{ check(physicsSystem); return *physicsSystem; }

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

void ModuleManager::setPhysicsSystem(PhysicsSystem* newPhysicsSystem)
{
	check(newPhysicsSystem);

	physicsSystem = newPhysicsSystem;
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



void ModuleManager::addInitCallback(const initFun& function)
{
	initCallbacks.push_back(function);
}

void ModuleManager::addUpdateCallback(const updateFun& function)
{
	updateCallbacks.push_back(function);
}

std::list<ModuleManager::initFun>& ModuleManager::getInitCallbacks()
{
	return initCallbacks;
}

std::list<ModuleManager::updateFun>& ModuleManager::getUpdateCallbacks()
{
	return updateCallbacks;
}