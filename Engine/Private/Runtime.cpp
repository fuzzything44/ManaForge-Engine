#include "Runtime.h"

#include "Helper.h"
#include "Window.h"
#include "ModuleManager.h"
#include "ChangeDirectory.h"

#include <functional>
#include <list>
#include <chrono>

#include <boost/algorithm/string.hpp>
#include <boost/timer.hpp>


// DEFINE STATIC VARIABLES
Runtime* Runtime::currentRuntime = nullptr;
bool Runtime::isInitalized = false;

Runtime::Runtime(const std::string& worldPath, const WindowProps& windowProps)
	: moduleManager(*this),
	propManager("props.json")
{
	
	if (!isInitalized)
	{
		FATAL_ERR("Runtime needs initalization before spawining one. Call Runtime::init() first");
	}

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

	
}

void Runtime::run()
{
	{
		// time the init time
		boost::timer t;

		std::list<std::function<void()> >& initCallbacks = moduleManager.getInitCallbacks();

		for (auto& callback : initCallbacks)
		{
			callback();
		}
		ENG_LOG("init completed. Timestamp: ");
	}
	

	// set initial tick
	float LastTick = static_cast<float>(clock::now().time_since_epoch().count());


	bool shouldContinue = true;

	do {

		
		// calculate tick time
		float CurrentTick = static_cast<float>(clock::now().time_since_epoch().count());
		float delta = CurrentTick - LastTick;

		LastTick = CurrentTick;

		// recieve the update callbacks
		std::list<std::function<bool()> >& updateCallbacks = moduleManager.getUpdateCallbacks();

		shouldContinue = true;

		for (auto& callback : updateCallbacks)
		{
			if(!callback())
			{
				shouldContinue = false;
			}
		}
		

	} while (shouldContinue);
}

void Runtime::init()
{
	// change directory to the Resources folder
	changeDir();
}

Runtime& Runtime::get()
{
	if (!currentRuntime)
	{
		FATAL_ERR("NO RUNTIME OBJECT!");
	}

	return *currentRuntime;
}