#pragma once
#include "Engine.h"

#include <boost/core/noncopyable.hpp>

#include <chrono>

class PlayerController;
class World;
class Pawn;
class Renderer;
class PhysicsSystem;
class AudioSystem;
class ModuleManager;
class PropertyManager;
class InputManager;
class TimerManager;

class Runtime : boost::noncopyable
{
public:
	ENGINE_API explicit Runtime(const path_t& world);

	ENGINE_API ~Runtime();

	ENGINE_API void run();

	inline static Runtime& get();

	// returns the old world
	inline World* setWorld(World* newWorld);

	inline float getDeltaTime();

	inline ModuleManager& getModuleManager();
	inline PropertyManager& getPropertyManager();
	inline InputManager& getInputManager();
	inline TimerManager& getTimerManager();

	inline Renderer& getRenderer();
	inline PhysicsSystem& getPhysicsSystem();
	inline AudioSystem& getAudioSystem();

	using clock = std::chrono::high_resolution_clock;

private:
	std::unique_ptr<ModuleManager> moduleManager;
	std::unique_ptr<PropertyManager> propManager;
	std::unique_ptr<InputManager> inputManager;
	std::unique_ptr<TimerManager> timerManager;

	std::unique_ptr<Renderer> renderer;
	std::unique_ptr<PhysicsSystem> physSystem;
	std::unique_ptr<AudioSystem> audioSystem;

public:
	std::unique_ptr<World> world;

	std::unique_ptr<PlayerController> controller;
	std::unique_ptr<Pawn> pawn;

private:
	std::string rendererModuleName;
	std::string physicsSystemModuleName;
	std::string audioSystemModuleName;

	std::string rendererName;
	std::string physicsSystemName;
	std::string audioSystemName;

	float deltaTime;

	ENGINE_API static Runtime* currentRuntime;
};

#include "World.h"

inline Runtime& Runtime::get()
{
	assert(currentRuntime);
	return *currentRuntime;
}

inline World* Runtime::setWorld(World* newWorld)
{
	std::swap(world, std::unique_ptr<World>{newWorld});

	return newWorld;
}

inline float Runtime::getDeltaTime() { return deltaTime; }

inline ModuleManager& Runtime::getModuleManager() { return *moduleManager; }

inline PropertyManager& Runtime::getPropertyManager() { return *propManager; }

inline InputManager& Runtime::getInputManager() { return *inputManager; }

inline TimerManager& Runtime::getTimerManager() { return *timerManager; }

inline Renderer& Runtime::getRenderer() { return *renderer; }

inline PhysicsSystem& Runtime::getPhysicsSystem() { return *physSystem; }

inline AudioSystem& Runtime::getAudioSystem() { return *audioSystem; }
