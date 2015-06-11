#include "EnginePCH.h"

#include "Runtime.h"

#include "Window.h"
#include "ModuleManager.h"
#include "ChangeDirectory.h"
#include "ImageLoader.h"
#include "MeshComponent.h"
#include "PhysicsComponent.h"

#include <functional>
#include <list>
#include <chrono>

#include <boost/timer/timer.hpp>

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

		std::list<ModuleManager::initFun>& initCallbacks = moduleManager.getInitCallbacks();

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
	Actor* gate = moduleManager.spawnClass<Actor>("TestContent.Gate");
	gate->setWorldLocation(vec2(2.f, 2.f));
	gate->setWorldRotation(.5f * (float)M_PI);
	
	uvec2 windowSize = moduleManager.getRenderer().getWindow().getWindowProps().size;

	float aspectRatio = static_cast<float>(windowSize.y) / static_cast<float>(windowSize.x);

	Actor* player = new Actor();

	CameraComponent* c = new CameraComponent{ player, Transform{}, aspectRatio, .1f };
	moduleManager.getRenderer().setCurrentCamera(c);


	vec2 locations[] = 
	{
		{ 0.f, 0.f },
		{ 0.f, 1.f },
		{ 1.f, 0.f },
		{ 1.f, 1.f }
	};
	vec2 UVs[] = 
	{
		{ 0.f, 1.f },
		{ 0.f, 0.f },
		{ 1.f, 1.f },
		{ 1.f, 0.f }
	};
	uvec3 tris[] = 
	{
		{ 0, 1, 2 },
		{ 1, 2, 3 }
	};

	Material* mat = moduleManager.getRenderer().newMaterial("boilerplate");
	Texture* tex = moduleManager.getRenderer().newTexture("4");
	mat->setTexture(0, tex);

	MeshComponent* meshComp = new MeshComponent(player, Transform{}, ModelData(locations, UVs, tris, 4, 2, mat));

	PhysicsShape* shape = moduleManager.getPhysicsSystem().newPhysicsShape();
	shape->asRectangle(1.f, 1.f);

	PhysicsComponent* physComp = new PhysicsComponent(player, Transform{}, shape);
	player->setPhysicsType(PhysicsType::DYNAMIC);
	
	// set initial tick
	clock::time_point LastTick = clock::now();


	bool shouldContinue = true;

	float baseSpeed = 10.f;

	do {

		ENG_LOGLN(static_cast<uint32>(gate->getPhysicsType()));

		// calculate tick time
		clock::time_point CurrentTick = clock::now();
		float delta = static_cast<float>((CurrentTick - LastTick).count()) * 
			((float)clock::time_point::duration::period::num / (float)clock::time_point::duration::period::den);

		LastTick = CurrentTick;

		inputManager.update();


		Window& window = moduleManager.getRenderer().getWindow();


		float speedUpdated = window.getIsKeyPressed(Keyboard::KEY_LEFT_SHIFT) || window.getIsKeyPressed(Keyboard::KEY_RIGHT_SHIFT) ?
			5.f * baseSpeed : baseSpeed;

		speedUpdated /= 100.f;

		vec2 velocity = player->getVelocity();

		if (window.getIsKeyPressed(Keyboard::KEY_Q))
		{
			c->setZoom(c->getZoom() * 1.001f);
		}
		if (window.getIsKeyPressed(Keyboard::KEY_E))
		{
			c->setZoom(c->getZoom() * (1.f / 1.001f));
		}
		if (window.getIsKeyPressed(Keyboard::KEY_A))
		{
			//player->addWorldLocation(vec2(-speedUpdated * delta, 0.f));
			velocity.x -= speedUpdated;
		}
		if (window.getIsKeyPressed(Keyboard::KEY_D))
		{
			//player->addWorldLocation(vec2(speedUpdated * delta, 0.f));
			velocity.x += speedUpdated;

		}
		if (window.getIsKeyPressed(Keyboard::KEY_W))
		{
			//player->addWorldLocation(vec2(0.f, speedUpdated * delta));
			velocity.y += speedUpdated;
		}
		if (window.getIsKeyPressed(Keyboard::KEY_S))
		{
			//player->addWorldLocation(vec2(0.f, -speedUpdated * delta));
			velocity.y -= speedUpdated;

		}

		if (glm::length(velocity) > 15.f)
		{
			velocity = glm::normalize(velocity) * 15.f;
		}

		velocity *= .99f;

		player->setVelocity(velocity);

		// recieve the update callbacks
		std::list<ModuleManager::updateFun>& updateCallbacks = moduleManager.getUpdateCallbacks();

		shouldContinue = true;

		for (auto& callback : updateCallbacks)
		{
			if (callback._Empty())
			{
				ENG_LOGLN("Warning: Update callback empty");
			}

			if(!callback(delta)) // it would return true if we should coninue
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

PlayerController* Runtime::addPlayerController()
{
	return world->makePlayerController();
}
