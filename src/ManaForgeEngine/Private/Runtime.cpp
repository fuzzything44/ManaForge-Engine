#include "EnginePCH.h"

#include "Runtime.h"

#include "WindowWidget.h"
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

#include "ModuleManager.h"
#include "PropertyManager.h"
#include "InputManager.h"
#include "TimerManager.h"

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
{
	changeDir();
	logdetail::log_base::init();

	moduleManager = std::make_unique<ModuleManager>();
	propManager = std::make_unique<PropertyManager>("props.json");
	inputManager = std::make_unique<InputManager>();
	timerManager = std::make_unique<TimerManager>();

	// update current runtime to be the most recently created one
	currentRuntime = this;

	std::string modulesStr;
	LOAD_PROPERTY_WITH_ERROR(getPropertyManager(), "modules", modulesStr);

	// split into the individual modules
	std::vector<std::string> modules;
	boost::algorithm::split(modules, modulesStr, boost::algorithm::is_any_of("\t\n "));

	// load modules from the property sheet
	for (auto& elem : modules) {
		if (elem != "" && elem != "\t" && elem != "\n") moduleManager->loadModule(elem);
	}

	LOAD_PROPERTY_WITH_ERROR(getPropertyManager(), "Renderer.Module", rendererModuleName);
	LOAD_PROPERTY_WITH_ERROR(getPropertyManager(), "AudioSystem.Module", audioSystemModuleName);
	LOAD_PROPERTY_WITH_ERROR(getPropertyManager(), "PhysicsSystem.Module", physicsSystemModuleName);

	LOAD_PROPERTY_WITH_ERROR(getPropertyManager(), "Renderer.Name", rendererName);
	LOAD_PROPERTY_WITH_ERROR(getPropertyManager(), "AudioSystem.Name", audioSystemName);
	LOAD_PROPERTY_WITH_ERROR(getPropertyManager(), "PhysicsSystem.Name", physicsSystemName);
}

Runtime::~Runtime() { logdetail::log_base::cleanup(); }

void Runtime::run()
{
	// create the systems.
	renderer =
		std::unique_ptr<Renderer>{getModuleManager().spawnClass<Renderer>(rendererModuleName, rendererName)};
	physSystem = std::unique_ptr<PhysicsSystem>{
		getModuleManager().spawnClass<PhysicsSystem>(physicsSystemModuleName, physicsSystemName)};
	audioSystem = std::unique_ptr<AudioSystem>{
		getModuleManager().spawnClass<AudioSystem>(audioSystemModuleName, audioSystemName)};
	assert(renderer);
	assert(physSystem);
	assert(audioSystem);

	{
		// get the callbacks
		auto initCallbacks = getModuleManager().getInitCallbacks();

		for (auto& callback : initCallbacks) {
			if (!callback) {
				MFLOG(Warning) << "init callback empty.";
			}

			callback();
		}
		MFLOG(Trace) << "init completed.";
	}

	// load the world
	world = std::unique_ptr<World>{getModuleManager().spawnClass<World>("DefaultWorld", "DefaultWorld")};
	world->init("default"); // load the test world

	// let the input manager know of the window
	WindowWidget& window = getRenderer().getWindow();
	getInputManager().setWindow(window);

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

		getInputManager().update();
		getTimerManager().update();

		WindowWidget& window = getRenderer().getWindow();

		// recieve the update callbacks
		auto updateCallbacks = getModuleManager().getUpdateCallbacks();

		shouldContinue = true;

		for (auto& callback : updateCallbacks) {
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
