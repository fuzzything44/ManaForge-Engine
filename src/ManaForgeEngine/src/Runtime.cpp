#include "EnginePCH.h"

#include "Runtime.h"
#include "ChangeDirectory.h"
#include "Logging.h"

ENGINE_API Runtime* Runtime::runtimeObj = nullptr;

DLLEXPORT Runtime::Runtime()
{

	runtimeObj = this;

	changeDir();
	logdetail::log_base::init();

	coreManager = CoreManager_t::factory();

	moduleHandler.init({ "OpenALAudioSystem", "Box2DPhysicsSystem", "OpenGLRenderer", "TestContent" });


}

DLLEXPORT Runtime::~Runtime() 
{ 
	logdetail::log_base::cleanup(); 
	
	delete coreManager;
}


DLLEXPORT void Runtime::run()
{
	coreManager->beginPlay();

	std::chrono::system_clock::time_point lastTick = std::chrono::system_clock::now();
	do
	{

		// calculate tick time
		std::chrono::system_clock::time_point currentTick = std::chrono::system_clock::now();
		std::chrono::duration<float> delta_duration = currentTick - lastTick;
		deltaTime = delta_duration.count();

		lastTick = currentTick;

		timerManager.update();

		coreManager->update();

	} while (shouldContinue);
}
