#include "Engine.h"
#include "ModuleManager.h"
#include "Helper.h"

#include <boost/algorithm/string.hpp>

ModuleManager* ModuleManager::currentMM = nullptr;

ModuleManager::~ModuleManager()
{
	delete renderer;
	delete createWorld;
}

ModuleManager::ModuleManager()
	: renderer(nullptr)
{
	currentMM = this;
	

	// load modules form file
	std::string file = loadFileToStr("modules.txt");

	std::vector<std::string> modules;
	boost::algorithm::split(modules, file, boost::algorithm::is_any_of("\t\n "));

	for (auto& elem : modules)
	{
		if (elem != "" && elem != "\t" && elem !="\n")
			loadModule(elem);
	}
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

void ModuleManager::addRenderer(Renderer* newRenderer)
{
	
	avaliableRenderers.push_back(newRenderer);

	if (!renderer)
	{
		renderer = newRenderer;
	}
}

void ModuleManager::setWorld(std::function<World*(std::string)> createWorldFun)
{
	
	// copy to the class's version
	createWorld = new std::function<World*(std::string)>(createWorldFun);
}

ModuleManager& ModuleManager::get()
{
	check(currentMM);

	return *currentMM;
}
