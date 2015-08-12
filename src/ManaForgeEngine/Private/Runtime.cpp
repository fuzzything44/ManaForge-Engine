#include "EnginePCH.h"

#include "Runtime.h"

#include "Window.h"
#include "ModuleManager.h"
#include "ChangeDirectory.h"
#include "MeshComponent.h"
#include "PhysicsComponent.h"
#include "PlayerController.h"
#include "Pawn.h"
#include "Renderer.h"
#include "PhysicsSystem.h"
#include "Texture.h"
#include "CameraComponent.h"
#include "MaterialInstance.h"
#include "ModelData.h"

#include <functional>
#include <list>
#include <chrono>
#include <future>

#include <boost/timer/timer.hpp>

#include <boost/algorithm/string.hpp>

#include <boost/archive/polymorphic_xml_oarchive.hpp>
#include <boost/serialization/list.hpp>

// DEFINE STATIC VARIABLES
Runtime* Runtime::currentRuntime = nullptr;

Runtime::Runtime(const path_t& worldPath)
	: propManager((changeDir(), logdetail::log_base::init(), "props.json"))
	, moduleManager()
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
		if (elem != "" && elem != "\t" && elem != "\n") moduleManager.loadModule(elem);
	}

	LOAD_PROPERTY_WITH_ERROR(propManager, "Renderer.Module", rendererModuleName);
	LOAD_PROPERTY_WITH_ERROR(propManager, "AudioSystem.Module", audioSystemModuleName);
	LOAD_PROPERTY_WITH_ERROR(propManager, "PhysicsSystem.Module", physicsSystemModuleName);

	LOAD_PROPERTY_WITH_ERROR(propManager, "Renderer.Name", rendererName);
	LOAD_PROPERTY_WITH_ERROR(propManager, "AudioSystem.Name", audioSystemName);
	LOAD_PROPERTY_WITH_ERROR(propManager, "PhysicsSystem.Name", physicsSystemName);
}

Runtime::~Runtime() { logdetail::log_base::cleanup(); }

void Runtime::run()
{
	// create the systems.
	renderer =
		std::unique_ptr<Renderer>{moduleManager.spawnClass<Renderer>(rendererModuleName, rendererName)};
	physSystem = std::unique_ptr<PhysicsSystem>{
		moduleManager.spawnClass<PhysicsSystem>(physicsSystemModuleName, physicsSystemName)};
	audioSystem = std::unique_ptr<AudioSystem>{
		moduleManager.spawnClass<AudioSystem>(audioSystemModuleName, audioSystemName)};
	assert(renderer);
	assert(physSystem);
	assert(audioSystem);

	{
		// get the callbacks
		auto initCallbacks = moduleManager.getInitCallbacks();

		for (auto& callback : initCallbacks)
		{
			if (!callback) {
				MFLOG(Warning) << "init callback empty.";
			}

			callback();
		}
		MFLOG(Trace) << "init completed.";
	}

	// load the world
	world = std::unique_ptr<World>{moduleManager.spawnClass<World>("DefaultWorld", "DefaultWorld")};
	world->init("default"); // load the test world

	// let the input manager know of the window
	Window& window = renderer->getWindow();
	inputManager.setWindow(window);

	// spawn the new playercontrollers and pawns
	controller = world->makePlayerController();
	pawn = world->makePawn();
	// assert(controller); TODO: Core Classes
	assert(pawn);

	// set initial tick
	clock::time_point lastTick = clock::now();

	bool shouldContinue = true;

	do
	{

		// calculate tick time
		clock::time_point currentTick = clock::now();
		std::chrono::duration<float> delta_duration = currentTick - lastTick;
		deltaTime = delta_duration.count();

		lastTick = currentTick;

		inputManager.update();
		timerManager.update();

		Window& window = renderer->getWindow();

		// recieve the update callbacks
		auto updateCallbacks = moduleManager.getUpdateCallbacks();

		shouldContinue = true;

		for (auto& callback : updateCallbacks)
		{
			if (!callback) {
				MFLOG(Warning) << "Update callback empty";
			}
			else if (!callback(deltaTime))
			{
				shouldContinue = false;
			}
		}

	} while (shouldContinue);
}
