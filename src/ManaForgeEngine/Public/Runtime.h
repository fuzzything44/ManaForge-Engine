#pragma once
#include "Engine.h"

#include <chrono>

#include "CoreManager.h"
#include "TimerManager.h"

class Runtime
{
public:

	Runtime();
	void run();

	~Runtime();

	std::shared_ptr<CoreManager_t> coreManager;

	TimerManager timerManager;

private:
	bool shouldContinue = true;

};
