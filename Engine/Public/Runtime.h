#pragma once

#include "Engine.h"
#include "PropertyManager.h"
#include "ModuleManager.h"

class Runtime
{
public:

	ENGINE_API Runtime();

	ModuleManager moduleManager;
	PropertyManager propManager;

	ENGINE_API static Runtime& get();

private:
	static Runtime* currentRuntime;
};