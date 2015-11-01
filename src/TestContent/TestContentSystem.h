#pragma once

#include <CoreManager.h>
#include <TestContentConfig.h>
#include <../../OpenALAudioSystem/Public/OpenALAudioSystem.h>
#include <../../Box2DPhysicsSystem/Public/Box2DPhysicsManager.h>

struct TPew{};


using TestContentManager_t =
	Manager
	<
		boost::mpl::vector0<>
		, boost::mpl::vector1<TPew>
		, boost::mpl::vector2<OpenALAudioManager_t, Box2DPhysicsManager_t>
	>;


template<>
void beginPlayManager<TestContentManager_t>(TestContentManager_t& manager)
{
	auto ent1 = manager.createEntity();

	b2BodyDef bdef;

	manager.addComponent<CBox2DCollision>(ent1, &manager.getRefToManager<Box2DPhysicsManager_t>().getManagerData().world, bdef);
	manager.addTag<TPew>(ent1);
}

template<>
void updateManager<TestContentManager_t>(TestContentManager_t& manager)
{
	manager.runAllMatching<boost::mpl::vector<CBox2DCollision>>([](CBox2DCollision& collision)
	{
		std::cout << "HEY!\t";
	});

	std::cout << std::endl;
}



extern TestContent_API std::weak_ptr<TestContentManager_t> testContentManager;


