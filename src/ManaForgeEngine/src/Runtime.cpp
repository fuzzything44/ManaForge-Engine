#include "EnginePCH.h"

#include "Runtime.h"
#include "Logging.h"

#include <cassert>

#include <QtCore>

ENGINE_API Runtime* Runtime::runtimeObj = nullptr;

DLLEXPORT Runtime::Runtime(int argc, char** argv)
	:application{argc, argv}
{
	// make sure that there is only one runtimeObj
	assert(!runtimeObj);
	
	runtimeObj = this;
	
	boost::filesystem::current_path(MF_RESOURCE_DIR);
	
	logdetail::log_base::init();

	coreManager = CoreManager_t::factory();

	moduleHandler.init({ "OpenALAudioSystem", "Box2DPhysicsSystem", "OpenGLRenderer", "TestContent" });
	
}

DLLEXPORT Runtime::~Runtime()
{
	logdetail::log_base::cleanup();

	delete coreManager;
}


DLLEXPORT int Runtime::run()
{
	
	
	coreManager->beginPlay();
	
	std::chrono::system_clock::time_point lastTick = std::chrono::system_clock::now();
	
	connect(&timer, &QTimer::timeout, 
		[&lastTick, this]
		{
			// calculate tick time
			std::chrono::system_clock::time_point currentTick = std::chrono::system_clock::now();
			std::chrono::duration<float> delta_duration = currentTick - lastTick;
			deltaTime = delta_duration.count();
			
			lastTick = currentTick;
			
			timerManager.update();
			
			coreManager->update();
			
		});
		
	return application.exec();
}
