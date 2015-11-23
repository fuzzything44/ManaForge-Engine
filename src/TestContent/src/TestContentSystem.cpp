#include "TestContentSystem.h"

#include <ModuleHandler.h>

TestContentManager_t* testContentManager = nullptr;

extern"C" DLLEXPORT void init(ModuleHandler& handler)
{
	// load dependent modules
	handler.loadModule("OpenALAudioSystem");
	handler.loadModule("OpenGLRenderer");
	handler.loadModule("Box2DPhysicsSystem");

	testContentManager = TestContentManager_t::factory
		(
			std::make_tuple(
				openALAudioManager
				, box2DPhysicsManager
				, openGLRendererManager
			)
		);
}

extern"C" DLLEXPORT void cleanup(ModuleHandler& handler)
{
	delete testContentManager;
	testContentManager = nullptr;
}