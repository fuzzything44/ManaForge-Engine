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


void ModuleManager::loadModule(const std::string& filename)
{

	if (loadedModules.find(filename) == loadedModules.end())
	{
		loadedModules.insert(std::map<std::string, Module>::value_type(filename, Module(filename))).first->second.registerModule(*this);
	}
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