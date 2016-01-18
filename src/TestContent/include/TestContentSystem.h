#pragma once

#include <CoreManager.h>
#include <TestContentConfig.h>
#include <OpenALAudioSystem.h>
#include <Box2DPhysicsManager.h>
#include <OpenGLRendererManager.h>

#include <QOpenGLFunctions_3_3_Core>

struct TPew{};


using TestContentManager_t =
	Manager
	<
		decltype(make_type_tuple<TPew>)
		, decltype(make_type_tuple<OpenALAudioManager_t, Box2DPhysicsManager_t, OpenGLRendererManager_t>)
	>;

template<typename... Args>
using TupleOfVectorRefrences = std::tuple<std::vector<Args>&...>;


template<>
void beginPlayManager<TestContentManager_t>(TestContentManager_t& manager)
{
	auto ent1 = manager.newEntity(make_type_tuple<CPosition, TPew>, boost::hana::make_tuple(CPosition{{.3f, 2.f}}));

	assert(manager.hasComponent(boost::hana::type_c<CPosition>, ent1));
	assert(manager.hasComponent(boost::hana::type_c<TPew>, ent1));
	assert(!manager.hasComponent(boost::hana::type_c<CVelocity>, ent1));
	assert(manager.getStorageComponent(boost::hana::type_c<CPosition>, ent1).value == vec2(.3f, 2.f));
	
	vec2 locs[] = 
	{
		{0.f, 0.f},
		{0.f, 1.f},
		{1.f, 0.f},
		{1.f, 1.f}
	};
	uvec3 indicies[] =
	{
		{0, 1, 2},
		{1, 2, 3}
	};
	
	auto&& funs = manager.getRefToManager(boost::hana::type_c<OpenGLRendererManager_t>).getManagerData().getFuncs();
	
	OpenGLModelData modelData{ &funs, locs, locs, 4, indicies, 2 };
	OpenGLMaterialSource matSource{ funs, "boilerplate" };
	auto mat = std::make_shared<OpenGLMaterialInstance>(matSource);
	COpenGLModel model{ modelData, mat };
	auto modelEnt = manager.newEntity(make_type_tuple<CPosition, COpenGLModel>, boost::hana::make_tuple(CPosition{ {0.f, 0.f} }, model));



}

template<>
void updateManager<TestContentManager_t>(TestContentManager_t& manager)
{
	manager.runAllMatching(make_type_tuple<CPosition, TPew>, [](CPosition& pos)
	{

	});
}



extern TestContent_API TestContentManager_t* testContentManager;


