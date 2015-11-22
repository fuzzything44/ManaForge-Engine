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
	auto ent1 = manager.newEntity<boost::mpl::vector1<CPosition>>(std::make_tuple(CPosition{{.3f, 2.f}}));

	//manager.addTag<TPew>(ent1);
	//assert(manager.hasTag<TPew>(ent1));
	//manager.removeTag<TPew>(ent1);

	//std::array<vec2, 4> locs = { {
	//	{ 0.f, 0.f },
	//	{ 0.f, 1.f },
	//	{ 1.f, 0.f },
	//	{ 1.f, 1.f }
	//} };

	//std::array<vec2, 4> UVs = { {
	//	{ 0.f, 1.f },
	//	{ 0.f, 0.f },
	//	{ 1.f, 1.f },
	//	{ 1.f, 0.f }
	//} };

	//std::array<uvec3, 2> tris = { {
	//	{ 0, 1, 2 },
	//	{ 1, 2, 3 }
	//} };

	//OpenGLModelData data{ locs.data(), UVs.data(), locs.size(), tris.data(), tris.size() };
	//OpenGLMaterialSource src{ "Boilerplate" };
	//std::shared_ptr<OpenGLMaterialInstance> inst = std::make_shared<OpenGLMaterialInstance>(src);

	//OpenGLTexture tex{ "4" };
	//inst->textures[0] = tex;

	//std::vector<CPosition> pos = { {{3.f, 1.f}},{{2.f, 1.f}} };

	//auto&& newEnts = manager.createEntityBatch<boost::mpl::vector<CPosition>>(std::forward_as_tuple(pos), pos.size());


}

template<>
void updateManager<TestContentManager_t>(TestContentManager_t& manager)
{
	manager.runAllMatching<boost::mpl::vector<CPosition, TPew>>([](CPosition& pos)
	{
		
	});
}



extern TestContent_API std::weak_ptr<TestContentManager_t> testContentManager;


