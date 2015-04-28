#pragma once

#include "Engine.h"
#include "PropertyManager.h"
#include "ModuleManager.h"

class Runtime
{
public:

	ENGINE_API explicit Runtime(std::string world = "");

	ModuleManager moduleManager;
	PropertyManager propManager;

	World* world;

	ENGINE_API static Runtime& get();

private:
	static Runtime* currentRuntime;
};