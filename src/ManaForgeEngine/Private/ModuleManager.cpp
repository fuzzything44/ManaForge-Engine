#include "EnginePCH.h"

#include "ModuleManager.h"

#include "Helper.h"
#include "AudioSystem.h"
#include "Module.h"
#include "PhysicsSystem.h"
#include "AudioSystem.h"
#include "Renderer.h"

#include <map>

ModuleManager::ModuleManager() {}

ModuleManager::~ModuleManager() {}


void ModuleManager::loadModule(const std::string& name)
{

	if (loadedModules.find(name) == loadedModules.end()) {
		loadedModules.insert({name, std::make_shared<Module>(name)}).first->second->registerModule(*this);
	}
	else
	{
		MFLOG(Warning) << "Module: " << name << " already loaded.";
	}
}


void ModuleManager::addInitCallback(const initFun& function) { initCallbacks.push_back(function); }

void ModuleManager::addUpdateCallback(const updateFun& function) { updateCallbacks.push_back(function); }

std::list<ModuleManager::initFun>& ModuleManager::getInitCallbacks() { return initCallbacks; }

std::list<ModuleManager::updateFun>& ModuleManager::getUpdateCallbacks() { return updateCallbacks; }