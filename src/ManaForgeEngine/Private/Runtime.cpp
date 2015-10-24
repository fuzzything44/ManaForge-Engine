#include "EnginePCH.h"

#include "Runtime.h"
#include "ChangeDirectory.h"
#include "Logging.h"

Runtime::Runtime()
{
	changeDir();
	logdetail::log_base::init();

	coreManager = CoreManager_t::factory();

}

Runtime::~Runtime() { logdetail::log_base::cleanup(); }

void Runtime::run()
{
	std::chrono::system_clock::time_point lastTick = std::chrono::system_clock::now();
	do
	{

		// calculate tick time
		std::chrono::system_clock::time_point currentTick = std::chrono::system_clock::now();
		std::chrono::duration<float> delta_duration = currentTick - lastTick;
		auto deltaTime = delta_duration.count();

		lastTick = currentTick;

		timerManager.update();

		shouldContinue = true;

		// some kind of update callbacks

	} while (shouldContinue);
}
