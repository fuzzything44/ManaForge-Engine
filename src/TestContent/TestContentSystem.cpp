#include "TestContentSystem.h"

std::weak_ptr<TestContentManager_t> testContentManager = {};

extern"C" __declspec(dllexport) void init()
{
	testContentManager = TestContentManager_t::factory(std::make_tuple(openALAudioManager.lock().get(), box2DPhysicsManager.lock().get()));
}
