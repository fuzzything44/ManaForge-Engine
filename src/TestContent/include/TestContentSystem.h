#pragma once

#include <CoreManager.h>
#include <TestContentConfig.h>
#include <OpenALAudioSystem/include/OpenALAudioSystem.h>
#include <Box2DPhysicsSystem/include/Box2DPhysicsManager.h>
#include <OpenGLRenderer/include/OpenGLRendererManager.h>

struct TPew{};


using TestContentManager_t =
	Manager
	<
		boost::mpl::vector1<TPew>
		, boost::mpl::vector3<OpenALAudioManager_t, Box2DPhysicsManager_t, OpenGLRendererManager_t>
	>;

template<typename... Args>
using TupleOfVectorRefrences = std::tuple<std::vector<Args>&...>;


template<>
void beginPlayManager<TestContentManager_t>(TestContentManager_t& manager)
{
	auto ent1 = manager.newEntity<boost::mpl::vector2<CPosition, TPew>>(std::make_tuple(CPosition{{.3f, 2.f}}));

    assert(manager.hasComponent<CPosition>(ent1));
    assert(manager.hasComponent<TPew>(ent1));
    assert(!manager.hasComponent<CVelocity>(ent1));
    assert(manager.getStorageComponent<CPosition>(ent1).value == vec2(.3f, 2.f));


}

template<>
void updateManager<TestContentManager_t>(TestContentManager_t& manager)
{
	manager.runAllMatching<boost::mpl::vector<CPosition, TPew>>([](CPosition& pos)
	{

	});
}



extern TestContent_API TestContentManager_t* testContentManager;


