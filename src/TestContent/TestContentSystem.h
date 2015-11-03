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
	OpenGLMaterialSource src{ "Boilerplate" };
	std::shared_ptr<OpenGLMaterialInstance> inst = std::make_shared<OpenGLMaterialInstance>(src);

	manager.addComponent<COpenGLModel>(ent1, data, inst);
	manager.addComponent<CPosition>(ent1) = { { 0.f, 0.f } };
}

template<>
void updateManager<TestContentManager_t>(TestContentManager_t& manager)
{
	manager.runAllMatching<boost::mpl::vector<TPew>>([]()
	{
		std::cout << "HEY!\t";
	});
}



extern TestContent_API std::weak_ptr<TestContentManager_t> testContentManager;


