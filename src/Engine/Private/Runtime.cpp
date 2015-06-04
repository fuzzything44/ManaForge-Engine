#include "Runtime.h"

#include "Window.h"
#include "ModuleManager.h"
#include "ChangeDirectory.h"

#include <functional>
#include <list>
#include <chrono>

#include <boost/timer/timer.hpp>

#include "ImageLoader.h"

#include <boost/algorithm/string.hpp>




// DEFINE STATIC VARIABLES
Runtime* Runtime::currentRuntime = nullptr;

Runtime::Runtime(const std::string& worldPath)
	:propManager((changeDir(), logging::init(), "props.json")),
	moduleManager()
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

	Window& window = moduleManager.getRenderer().getWindow();
	inputManager.setWindow(window);
}

Runtime::~Runtime()
{
	ImageLoader::cleanUp();

	delete world;
}

void Runtime::run()
{
	{

		std::list<std::function<void()> >& initCallbacks = moduleManager.getInitCallbacks();

		for (auto& callback : initCallbacks)
		{
			if (callback._Empty())
			{
				ENG_LOGLN("Warning: init callback empty.");
			}

			callback();
		}
		ENG_LOGLN("init completed.");

		
	}

	// YA WE REALLY NEED PLAYER CONTROLLERS -- the gate shouldn't get to control the camera
	Actor* gate = moduleManager.spawnActor("TestContent.Gate", Transform{ });

	uvec2 windowSize = moduleManager.getRenderer().getWindow().getWindowProps().size;

	float aspectRatio = static_cast<float>(windowSize.y) / static_cast<float>(windowSize.x);

	CameraComponent* c = new CameraComponent{ gate, Transform{}, aspectRatio, 1.f };
	moduleManager.getRenderer().setCurrentCamera(c);
	
	// set initial tick
	clock::time_point LastTick = clock::now();


	bool shouldContinue = true;

	float baseSpeed = 100.f;

	do {

		// calculate tick time
		clock::time_point CurrentTick = clock::now();
		float delta = static_cast<float>((CurrentTick - LastTick).count()) * 
			((float)clock::time_point::duration::period::num / (float)clock::time_point::duration::period::den);

		LastTick = CurrentTick;

		inputManager.update();


		Window& window = moduleManager.getRenderer().getWindow();


		float speedUpdated = window.getIsKeyPressed(Keyboard::KEY_LEFT_SHIFT) || window.getIsKeyPressed(Keyboard::KEY_RIGHT_SHIFT) ?
			5.f * baseSpeed : baseSpeed;

		if (window.getIsKeyPressed(Keyboard::KEY_Q))
		{
			c->setZoom(c->getZoom() * 1.01f);
		}
		if (window.getIsKeyPressed(Keyboard::KEY_E))
		{
			c->setZoom(c->getZoom() * (1.f / 1.01f));
		}
		if (window.getIsKeyPressed(Keyboard::KEY_A))
		{
			c->setLocation(c->getLocalLocation() + vec2(speedUpdated * delta, 0.f));
		}
		if (window.getIsKeyPressed(Keyboard::KEY_D))
		{
			c->setLocation(c->getLocalLocation() + vec2(-speedUpdated * delta, 0.f));
		}
		if (window.getIsKeyPressed(Keyboard::KEY_W))
		{
			c->setLocation(c->getLocalLocation() + vec2(0.f, -speedUpdated * delta));
		}
		if (window.getIsKeyPressed(Keyboard::KEY_S))
		{
			c->setLocation(c->getLocalLocation() + vec2(0.f, speedUpdated * delta));
		}


		// recieve the update callbacks
		std::list<std::function<bool()> >& updateCallbacks = moduleManager.getUpdateCallbacks();

		shouldContinue = true;

		for (auto& callback : updateCallbacks)
		{
			if (callback._Empty())
			{
				ENG_LOGLN("Warning: Update callback empty");
			}

			if(!callback()) // it would return true if we should coninue
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