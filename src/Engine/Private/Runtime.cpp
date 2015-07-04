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

#include <functional>
#include <list>
#include <chrono>

#include <boost/timer/timer.hpp>

#include <boost/algorithm/string.hpp>


// DEFINE STATIC VARIABLES
Runtime* Runtime::currentRuntime = nullptr;

Runtime::Runtime(const std::string& worldPath)
	:propManager((changeDir(), logdetail::sink_t::init(), "props.json")),
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

	// load the world
	world = moduleManager.spawnClass<World>("DefaultWorld.DefaultWorld");
	world->init("default");// load the test world

	Window& window = moduleManager.getRenderer().getWindow();
	inputManager.setWindow(window);
}

Runtime::~Runtime()
{
	ImageLoader::cleanUp();
	logdetail::log_base::cleanup();

	delete world;

}

void Runtime::run()
{
	{
		// get the callbacks
		std::list<ModuleManager::initFun>& initCallbacks = moduleManager.getInitCallbacks();

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

	// YA WE REALLY NEED PLAYER CONTROLLERS -- the gate shouldn't get to control the camera
	Actor* gate = moduleManager.spawnClass<Actor>("TestContent.Gate");
	gate->setWorldLocation(vec2(2.f, 2.f));
	gate->setWorldRotation(.25f * (float)M_PI);
	
	uvec2 windowSize = moduleManager.getRenderer().getWindow().getWindowProps().size;

	float aspectRatio = static_cast<float>(windowSize.y) / static_cast<float>(windowSize.x);

	Actor* player = new Actor();

	CameraComponent* c = new CameraComponent{ *player, Transform{}, aspectRatio, .1f };
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
	auto tex = moduleManager.getRenderer().newTexture("4");
	mat->setTexture(0, *tex);

	MeshComponent* meshComp = new MeshComponent(*player, Transform{}, ModelData(locations, UVs, tris, 4, 2, mat));

	auto shape = moduleManager.getPhysicsSystem().newPhysicsShape();
	shape->setPosition(vec2(.5f, .5f));
	shape->asRectangle(.5f, .5f);

	PhysicsComponent* physComp = new PhysicsComponent(*player, *shape, Transform{});
	player->setPhysicsType(PhysicsType::DYNAMIC);
	physComp->setDensity(1.f);
	
	// set initial tick
	clock::time_point LastTick = clock::now();


	bool shouldContinue = true;

	float baseSpeed = 10.f;

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
	

		vec2 vel = player->getVelocity();
		if (glm::length(vel) >= 20.f)
		{
			vel = 20.f / glm::length(vel) * vel;
		}

		vel *= .99f;

		player->setVelocity(vel);


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
			player->applyLocalForce(vec2(-speedUpdated, 0.f), vec2(0.f, 0.f));
		}
		if (window.getIsKeyPressed(Keyboard::KEY_D))
		{
			player->applyLocalForce(vec2(speedUpdated, 0.f), vec2(0.f, 0.f));

		}
		if (window.getIsKeyPressed(Keyboard::KEY_W))
		{
			player->applyLocalForce(vec2(0.f, speedUpdated), vec2(0.f, 0.f));

		}
		if (window.getIsKeyPressed(Keyboard::KEY_S))
		{
			player->applyLocalForce(vec2(0.f, -speedUpdated), vec2(0.f, 0.f));

		}
		if (window.getIsKeyPressed(Keyboard::KEY_TAB))
		{
			player->applyTorque(-1.f);
		}
		if (window.getIsKeyPressed(Keyboard::KEY_R))
		{
			player->applyTorque(1.f);
		}

		gate->applyTorque(1.f);

		// recieve the update callbacks
		std::list<ModuleManager::updateFun>& updateCallbacks = moduleManager.getUpdateCallbacks();

		shouldContinue = true;

		for (auto& callback : updateCallbacks)
		{
			if (callback._Empty())
			{
				logger<Warning>() << "Update callback empty";
			}

			if(!callback(delta)) // it would return true if we should coninue
			{
				shouldContinue = false;
			}
		}
		


	} while (shouldContinue);

	delete mat;
}

