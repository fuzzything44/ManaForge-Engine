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
class Window;

class Runtime : boost::noncopyable
{
public:

	ENGINE_API explicit Runtime(const std::string& world = "");

	ENGINE_API ~Runtime();

	ENGINE_API void run();

	inline static Runtime& get();

	// returns the old world
	inline World* setWorld(World* newWorld);

	inline PlayerController* addPlayerController();

	ModuleManager moduleManager;
	PropertyManager propManager;
	InputManager inputManager;
	
	World* world;
private:

	typedef std::chrono::high_resolution_clock clock;

	static Runtime* currentRuntime;
};

// collapeed for quicker debugging -- not very important
inline Runtime& Runtime::get()
{if (!currentRuntime) { FATAL_ERR("NO RUNTIME OBJECT!"); } return *currentRuntime; }


inline PlayerController* Runtime::addPlayerController()
{
	return world->makePlayerController();
}

inline World* Runtime::setWorld(World* newWorld)
{
	std::swap(world, newWorld);

	return newWorld;
}