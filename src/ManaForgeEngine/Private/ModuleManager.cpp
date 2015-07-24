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
{

}

ModuleManager::~ModuleManager()
{

}


void ModuleManager::loadModule(const path_t& filename)
{

	if (loadedModules.find(filename) == loadedModules.end())
	{
		loadedModules.insert({ filename, std::make_shared<Module>(filename) }).first->second->registerModule(*this);
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