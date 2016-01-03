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
	
	//auto&& funs = manager.getRefToManager<OpenGLRendererManager_t>().getManagerData().getFuncs();
	
	//OpenGLModelData modelData{ funs, locs, locs, 4, indicies, 2 };
	//OpenGLMaterialSource matSource{ funs, "boilerplate" };
	//auto mat = std::make_shared<OpenGLMaterialInstance>(matSource);
	//COpenGLModel model{ modelData, mat };
	//auto modelEnt = manager.newEntity<boost::mpl::vector2<CPosition, COpenGLModel>>(std::make_tuple(CPosition{ {0.f, 0.f} }, model));



}

template<>
void updateManager<TestContentManager_t>(TestContentManager_t& manager)
{
	manager.runAllMatching<boost::mpl::vector<CPosition, TPew>>([](CPosition& pos)
	{

	});
}



extern TestContent_API TestContentManager_t* testContentManager;


