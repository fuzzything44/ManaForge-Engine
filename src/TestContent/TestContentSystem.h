#pragma once

#include <CoreManager.h>
#include <TestContentConfig.h>
#include <../../OpenALAudioSystem/Public/OpenALAudioSystem.h>
#include <../../Box2DPhysicsSystem/Public/Box2DPhysicsManager.h>
#include <../../OpenGLRenderer/Public/OpenGLRendererManager.h>

struct TPew{};


using TestContentManager_t =
	Manager
	<
		boost::mpl::vector0<>
		, boost::mpl::vector1<TPew>
		, boost::mpl::vector3<OpenALAudioManager_t, Box2DPhysicsManager_t, OpenGLRendererManager_t>
	>;


template<>
void beginPlayManager<TestContentManager_t>(TestContentManager_t& manager)
{
	auto ent1 = manager.createEntity();

	b2BodyDef bdef;

	manager.addComponent<CBox2DCollision>(ent1, &manager.getRefToManager<Box2DPhysicsManager_t>().getManagerData().world, bdef);
	manager.addTag<TPew>(ent1);
	assert(manager.hasTag<TPew>(ent1));
	manager.removeComponent<CBox2DCollision>(ent1);
	

	if(manager.hasComponent<CBox2DCollision>(ent1)) 
		manager.getComponent<CBox2DCollision>(ent1).body->GetAngle();

	manager.destroyEntity(ent1);

	std::vector<vec2> locs = { 
		{ 0.f, 0.f }, 
		{ 0.f, 1.f }, 
		{ 1.f, 0.f }, 
		{ 1.f, 1.f } 
	};

	std::vector<uvec3> tris = { 
		{ 0, 1, 2 }, 
		{ 1, 2, 3 } 
	};

	OpenGLModelData data{ locs.data(), locs.data(), locs.size(), tris.data(), tris.size() };

	ent1 = manager.createEntity();
	manager.addComponent<COpenGLModel>(ent1, data);
}

template<>
void updateManager<TestContentManager_t>(TestContentManager_t& manager)
{
	manager.runAllMatching<boost::mpl::vector<TPew>>([]()
	{
		std::cout << "HEY!\t";
	});

	std::cout << std::endl;
}



extern TestContent_API std::weak_ptr<TestContentManager_t> testContentManager;


