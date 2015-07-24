#pragma once
#include "Engine.h"

#include "SaveData.h"
#include "PropertyManager.h"
#include "ModuleManager.h"
#include "WindowProps.h"
#include "InputManager.h"

#include <boost/core/noncopyable.hpp>

#include <chrono>

class PlayerController;
class Pawn;
class Window;

class Runtime : boost::noncopyable
{
public:

	ENGINE_API explicit Runtime(const path_t& world);

	ENGINE_API ~Runtime();

	ENGINE_API void run();

	inline static Runtime& get();

	// returns the old world
	inline World* setWorld(World* newWorld);
	

	ModuleManager moduleManager;
	PropertyManager propManager;
	InputManager inputManager;

	std::unique_ptr<Renderer> renderer;
	std::unique_ptr<PhysicsSystem> physSystem;
	std::unique_ptr<AudioSystem> audioSystem;


	using clock = std::chrono::high_resolution_clock;

	std::unique_ptr<World> world;

	std::unique_ptr<PlayerController> controller;
	std::unique_ptr<Pawn> pawn;

private:
	std::string rendererName;
	std::string physicsSystemName;
	std::string audioSystemName;

	ENGINE_API static Runtime* currentRuntime;
};

#include "World.h"

// collapeed for quicker debugging -- not very important
inline Runtime& Runtime::get()
{check(currentRuntime); return *currentRuntime;}

inline World* Runtime::setWorld(World* newWorld)
{
	std::swap(world, std::unique_ptr < World > {newWorld});

	return newWorld;
}