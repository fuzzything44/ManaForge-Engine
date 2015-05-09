#pragma once

#include <boost/core/noncopyable.hpp>

#include "Engine.h"
#include "PropertyManager.h"
#include "ModuleManager.h"
#include "WindowProps.h"

#include <chrono>

class Window;

class Runtime :
	boost::noncopyable
{
public:

	ENGINE_API explicit Runtime(const std::string& world = "", const WindowProps& windowProps = WindowProps());

	ModuleManager moduleManager;
	PropertyManager propManager;
	
	bool renders;

	World* world;

	ENGINE_API static Runtime& get();

	ENGINE_API void run();

	ENGINE_API static void init();

private:

	typedef std::chrono::high_resolution_clock clock;

	static bool isInitalized;

	static Runtime* currentRuntime;
};
