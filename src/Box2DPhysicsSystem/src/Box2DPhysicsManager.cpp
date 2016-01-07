#include "Box2DPhysicsManager.h"

#include <ModuleHandler.h>

DLLEXPORT Box2DPhysicsManager_t* box2DPhysicsManager = nullptr;


extern "C" DLLEXPORT void init(ModuleHandler&)
{
	
	box2DPhysicsManager = new Box2DPhysicsManager_t(boost::hana::make_tuple(coreManager));
}


extern"C" DLLEXPORT void cleanup(ModuleHandler& handler)
{
	delete box2DPhysicsManager;
	box2DPhysicsManager = nullptr;
}



