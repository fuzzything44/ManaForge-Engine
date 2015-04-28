#include "Runtime.h"

#include "Helper.h"
#include <boost/algorithm/string.hpp>

Runtime* Runtime::currentRuntime = nullptr;

Runtime::Runtime(std::string worldPath)
	: propManager("props.json")
{
	

	std::string modulesStr = propManager.queryValue<std::string>("modules");

	std::vector<std::string> modules;
	boost::algorithm::split(modules, modulesStr, boost::algorithm::is_any_of("\t\n "));

	// load elements from the property sheet
	for (auto& elem : modules)
	{
		if (elem != "" && elem != "\t" && elem != "\n")
			moduleManager.loadModule(elem);
	}

	world = moduleManager.newWorld(worldPath);

}

Runtime& Runtime::get()
{
	if (!currentRuntime)
	{
		FATAL_ERR("NO RUNTIME OBJECT!");
	}

	return *currentRuntime;
}