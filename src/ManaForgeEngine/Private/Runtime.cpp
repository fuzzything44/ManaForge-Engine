#include "EnginePCH.h"

#include "Runtime.h"

#include "Window.h"
#include "ModuleManager.h"
#include "ChangeDirectory.h"
#include "ImageLoader.h"
#include "MeshComponent.h"
#include "PhysicsComponent.h"
#include "Renderer.h"
#include "PhysicsSystem.h"
#include "Texture.h"
#include "CameraComponent.h"
#include "Material.h"

#include <functional>
#include <list>
#include <chrono>
#include <future>

#include <boost/timer/timer.hpp>

#include <boost/algorithm/string.hpp>


// DEFINE STATIC VARIABLES
Runtime* Runtime::currentRuntime = nullptr;

Runtime::Runtime(const std::string& worldPath)
	:propManager((changeDir(), logdetail::log_base::init(), "props.json")),
	moduleManager()
{
	
	// update current runtime to be the most recently created one
	currentRuntime = this;

	std::string modulesStr;	
	LOAD_PROPERTY_WITH_ERROR(propManager, "modules", modulesStr)

	// split into the individual modules
	std::vector<std::string> modules;
	boost::algorithm::split(modules, modulesStr, boost::algorithm::is_any_of("\t\n "));

	// load modules from the property sheet
	for (auto& elem : modules)
	{
		if (elem != "" && elem != "\t" && elem != "\n")
			moduleManager.loadModule(elem);
	}

}

Runtime::~Runtime()
{
	ImageLoader::cleanUp();
	logdetail::log_base::cleanup();

}

void Runtime::run()
{


	{
		// get the callbacks
		auto initCallbacks = moduleManager.getInitCallbacks();

		for (auto& callback : initCallbacks)
		{
			if (callback._Empty())
			{
				logger<Warning>() << "init callback empty.";
			}

			callback();
		}
		logger<Trace>() << "init completed.";

		
	}


	// load the world
	world = std::unique_ptr < World > {moduleManager.spawnClass<World>("DefaultWorld.DefaultWorld")};
	world->init("default");// load the test world


	// let the input manager know of the window
	Window& window = moduleManager.getRenderer().getWindow();
	inputManager.setWindow(window);

	// spawn the new playercontrollers and pawns
	controller = world->makePlayerController();
	pawn = world->makePawn();
	
	// set initial tick
	clock::time_point LastTick = clock::now();


	bool shouldContinue = true;

	float baseSpeed = 100.f;

	do {
		// calculate tick time
		clock::time_point CurrentTick = clock::now();
		std::chrono::duration<float> delta_duration = CurrentTick - LastTick;
		float delta = delta_duration.count();

		LastTick = CurrentTick;

		inputManager.update();


		Window& window = moduleManager.getRenderer().getWindow();


		// recieve the update callbacks
		auto updateCallbacks = moduleManager.getUpdateCallbacks();

		shouldContinue = true;

		for (auto& callback : updateCallbacks)
		{
			if (callback._Empty())
			{
				logger<Warning>() << "Update callback empty";
			}
			else if (!callback(delta))
			{
				shouldContinue = false;
			}
			
		}
		
	

	} while (shouldContinue);
}

