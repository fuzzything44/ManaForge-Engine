#pragma once
#include "Engine.h"

#include <chrono>

#include "CoreManager.h"
#include "TimerManager.h"
#include "ModuleHandler.h"

class Runtime
{
public:

	ENGINE_API Runtime();
	ENGINE_API void run();

	ENGINE_API ~Runtime();

	TimerManager timerManager;
	ModuleHandler moduleHandler;

private:
	bool shouldContinue = true;

};
