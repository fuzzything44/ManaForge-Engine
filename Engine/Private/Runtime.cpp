#include "Runtime.h"

#include "Helper.h"
#include "Window.h"
#include <boost/algorithm/string.hpp>

// DEFINE STATIC VARIABLES
Runtime* Runtime::currentRuntime = nullptr;

Runtime::Runtime(const std::string& worldPath, const WindowProps& windowProps)
	: propManager("props.json"),
	
{
	
	// query the property manager to get the modules
	std::string modulesStr = propManager.queryValue<std::string>("modules");

	// split into the individual modules
	std::vector<std::string> modules;
	boost::algorithm::split(modules, modulesStr, boost::algorithm::is_any_of("\t\n "));

	// load modules from the property sheet
	for (auto& elem : modules)
	{
		if (elem != "" && elem != "\t" && elem != "\n")
			moduleManager.loadModule(elem);
	}

	// load the world
	world = moduleManager.newWorld(worldPath);

	// if we render, then start a window
	if (renders)
	{
		window = new Window(windowProps);
	}

}

void Runtime::run()
{
	
}

Runtime& Runtime::get()
{
	if (!currentRuntime)
	{
		FATAL_ERR("NO RUNTIME OBJECT!");
	}

	return *currentRuntime;
}