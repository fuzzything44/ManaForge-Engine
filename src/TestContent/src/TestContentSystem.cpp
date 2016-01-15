#include "TestContentSystem.h"

#include <ModuleHandler.h>

TestContentManager_t* testContentManager = nullptr;


extern"C" DLLEXPORT void mf_init_module(ModuleHandler& handler)
{
	
	// load dependent modules
	handler.loadModule("OpenALAudioSystem");
	handler.loadModule("OpenGLRenderer");
	handler.loadModule("Box2DPhysicsSystem");

	testContentManager = new TestContentManager_t
		(
			boost::hana::make_tuple(
				openALAudioManager
				, box2DPhysicsManager
				, openGLRendererManager
			)
		);
}

extern"C" DLLEXPORT void mf_cleanup_module(ModuleHandler& handler)
{
	delete testContentManager;
	testContentManager = nullptr;
}