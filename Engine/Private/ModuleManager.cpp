#include "Engine.h"
#include "ModuleManager.h"
#include "Helper.h"

#include <boost/foreach.hpp>
#include <map>

ModuleManager::~ModuleManager()
{
	BOOST_FOREACH(auto& elem, initCallbacks)
	{
		delete elem;
	}

	BOOST_FOREACH(auto& elem, updateCallbacks)
	{
		delete elem;
	}

	delete renderer;
	delete createWorld;
}

ModuleManager::ModuleManager(Runtime& runtime)
	: renderer(nullptr)
{

}

Renderer& ModuleManager::getRenderer()
{
	check(renderer);

    return *renderer;
	
}

void ModuleManager::loadModule(const std::string& filename)
{
	
	if (loadedModules.find(filename) == loadedModules.end())
	{
		loadedModules.insert(std::map<std::string, Module>::value_type(filename, Module(filename))).first->second.registerModule(*this);
	}
}

void ModuleManager::setRenderer(Renderer* newRenderer)
{
	check(newRenderer);

	renderer = newRenderer;
	
}

void ModuleManager::setWorld(std::function<World*(std::string)> createWorldFun)
{
	
	// copy to the class's version
	createWorld = new std::function<World*(std::string)>(createWorldFun);
}

World* ModuleManager::newWorld(std::string path)
{
	check(createWorld);

	return (*createWorld)(path);
}

void ModuleManager::AddContentModule(contentModuleSpawnFun fun, const std::string& moduleName)
{
	// add the function to the spawn method map
	contentSpawnMethods.insert(std::pair<std::string, contentModuleSpawnFun>(moduleName, fun));
}

Actor* ModuleManager::spawnActor(const std::string& name, const Transform& trans)
{

	std::vector<std::string> brokenName;
	boost::algorithm::split(brokenName, name, boost::algorithm::is_any_of("."));

	// make sure we have at least two elements
	if (brokenName.size() < 2) return nullptr;

	auto iter = contentSpawnMethods.find(brokenName[0]);

	// if it existes call it
	if (iter != contentSpawnMethods.end())
	{
		// call the function
		return iter->second(brokenName[1], trans);
	}
	else
	{
		// and if not then return nullptr
		return nullptr;
	}
}

void ModuleManager::addInitCallback(const std::function<void()>& function)
{
	initCallbacks.push_back(new std::function<void()>(function));
}

void ModuleManager::addUpdateCallback(const std::function<bool()>& function)
{
	updateCallbacks.push_back(new std::function<bool()>(function));
}

std::list<std::function<void()>* >& ModuleManager::getInitCallbacks()
{
	return initCallbacks;
}

std::list<std::function<bool()>* >& ModuleManager::getUpdateCallbacks()
{
	return updateCallbacks;
}