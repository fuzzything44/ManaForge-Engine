#pragma once

#include "Engine.h"
#include "SaveData.h"
#include "PropertyManager.h"
#include "ModuleManager.h"
#include "WindowProps.h"

#include <boost/core/noncopyable.hpp>

#include <chrono>
#include "Input.h"

class Window;

class Runtime :
	boost::noncopyable
{
public:

	ENGINE_API explicit Runtime(const std::string& world = "", const WindowProps& windowProps = WindowProps());

	ModuleManager moduleManager;
	PropertyManager propManager;
	InputManager inputManager;
	
	bool renders;

	World* world;

	ENGINE_API static Runtime& get();

	ENGINE_API void run();
private:

	typedef std::chrono::high_resolution_clock clock;

	static bool isInitalized;

	static Runtime* currentRuntime;
};
