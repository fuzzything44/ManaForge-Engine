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

	ENGINE_API static Runtime& get();

	ENGINE_API PlayerController* addPlayerController();

	ModuleManager moduleManager;
	PropertyManager propManager;
	InputManager inputManager;
	
	World* world;
private:

	typedef std::chrono::high_resolution_clock clock;

	static Runtime* currentRuntime;
};
