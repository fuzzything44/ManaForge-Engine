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

	void requestExit()
	{
		shouldContinue = false;
	}

	static Runtime& get()
	{
		return *runtimeObj;
	}

	float getDeltaTime()
	{
		return deltaTime;
	}

private:
	ENGINE_API static Runtime* runtimeObj;

	CoreManager_t* manager;

	bool shouldContinue = true;

	float deltaTime;

};
