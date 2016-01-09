#include "EnginePCH.h"



#include "Runtime.h"
#include "Logging.h"

#include <cassert>

#include <QtCore>

MF_API Runtime* Runtime::runtimeObj = nullptr;

DLLEXPORT Runtime::Runtime(int& argc, char**& argv)
	:application{argc, argv}
{
	// make sure that there is only one runtimeObj
	assert(!runtimeObj);
	
	runtimeObj = this;
	
	boost::filesystem::current_path(MF_RESOURCE_DIR);
	
	coreManager = new CoreManager_t;

	moduleHandler.init(std::vector<path_t>{ path_t{"OpenGLRenderer"} });
	
}

DLLEXPORT Runtime::~Runtime()
{

	delete coreManager;
}


DLLEXPORT int Runtime::run()
{
	
	
	
	std::chrono::system_clock::time_point lastTick = std::chrono::system_clock::now();
	
	timer.connect(&timer, &QTimer::timeout, 
		[&lastTick, this]
		{
			static bool b = false;
			if(!b) coreManager->beginPlay();
			b = true;
			
			// calculate tick time
			std::chrono::system_clock::time_point currentTick = std::chrono::system_clock::now();
			std::chrono::duration<float> delta_duration = currentTick - lastTick;
			deltaTime = delta_duration.count();
			
			lastTick = currentTick;
			
			timerManager.update();
			
			coreManager->update();
			
		});
	timer.start(0);
	
	return application.exec();
} 
