#include "TestContentSystem.h"

std::shared_ptr<TestContentManager_t> testContentManager{ nullptr };

extern"C" __declspec(dllexport) void init()
{
	testContentManager = TestContentManager_t::factory(std::make_tuple(openALAudioManager, box2DPhysicsManager));
}
