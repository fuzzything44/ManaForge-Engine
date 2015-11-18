#include "EnginePCH.h"

#include "Runtime.h"
#include "ChangeDirectory.h"
#include "Logging.h"

ENGINE_API Runtime* Runtime::runtimeObj = nullptr;

Runtime::Runtime()
{

	runtimeObj = this;

	changeDir();
	logdetail::log_base::init();

	manager = CoreManager_t::factory();
	coreManager = manager;

	moduleHandler.init({ "OpenALAudioSystem", "Box2DPhysicsSystem", "OpenGLRenderer", "TestContent" });


}

Runtime::~Runtime() { logdetail::log_base::cleanup(); }

void Runtime::run()
{
	manager->beginPlay();

	std::chrono::system_clock::time_point lastTick = std::chrono::system_clock::now();
	do
	{

		// calculate tick time
		std::chrono::system_clock::time_point currentTick = std::chrono::system_clock::now();
		std::chrono::duration<float> delta_duration = currentTick - lastTick;
		deltaTime = delta_duration.count();

		lastTick = currentTick;

		timerManager.update();

		manager->update();

	} while (shouldContinue);
}
