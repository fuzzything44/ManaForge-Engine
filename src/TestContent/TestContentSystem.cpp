#include "TestContentSystem.h"

#include <ModuleHandler.h>

std::weak_ptr<TestContentManager_t> testContentManager = {};

extern"C" __declspec(dllexport) void init(ModuleHandler& handler)
{
	// load dependent modules
	handler.loadModule("OpenALAudioSystem");
	handler.loadModule("OpenGLRenderer");
	handler.loadModule("Box2DPhysicsSystem");

	testContentManager = TestContentManager_t::factory
		(
			std::make_tuple(
				openALAudioManager.lock().get()
				, box2DPhysicsManager.lock().get()
				, openGLRendererManager.lock().get()
			)
		);
}
