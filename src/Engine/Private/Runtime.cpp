#include "Runtime.h"

#include "Window.h"
#include "ModuleManager.h"
#include "ChangeDirectory.h"

#include <functional>
#include <list>
#include <chrono>

#include <boost/algorithm/string.hpp>

#include <boost/timer/timer.hpp>



// DEFINE STATIC VARIABLES
Runtime* Runtime::currentRuntime = nullptr;

Runtime::Runtime(const std::string& worldPath, const WindowProps& windowProps)
	:moduleManager((changeDir(), logging::init(), *this)), 
	propManager("props.json")
{
	// update current runtime to be the most recently created one
	currentRuntime = this;

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

	moduleManager.getRenderer().getWindow().setWindowProps(windowProps);
}

void Runtime::run()
{
	{
		// time the init time
		boost::timer::cpu_timer t;

		std::list<std::function<void()>* >& initCallbacks = moduleManager.getInitCallbacks();

		for (auto& callback : initCallbacks)
		{
			(*callback)();
		}
		ENG_LOGLN("init completed. Timestamp: " << t.format());

		
	}

	// YA WE REALLY NEED PLAYER CONTROLLERS -- the gate shouldn't get to control the camera
	Actor* gate = moduleManager.spawnActor("TestContent.Gate");

	mat4 mat = glm::ortho(-10.f, 10.f, -10.f, 10.f, .1f, 100.f);
	CameraComponent* c = new CameraComponent{gate, Transform{}, mat};
	moduleManager.getRenderer().setCurrentCamera(c);

	// set initial tick
	clock::time_point LastTick = clock::now();


	bool shouldContinue = true;

	do {

		
		// calculate tick time
		clock::time_point CurrentTick = clock::now();
		float delta = static_cast<float>((CurrentTick - LastTick).count());

		LastTick = CurrentTick;

		// recieve the update callbacks
		std::list<std::function<bool()>* >& updateCallbacks = moduleManager.getUpdateCallbacks();

		shouldContinue = true;

		for (auto& callback : updateCallbacks)
		{
			if(!(*callback)())
			{
				shouldContinue = false;
			}
		}
		

	} while (shouldContinue);
}

Runtime& Runtime::get()
{
	if (!currentRuntime)
	{
		FATAL_ERR("NO RUNTIME OBJECT!");
	}

	return *currentRuntime;
}