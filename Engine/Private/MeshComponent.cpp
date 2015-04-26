#include "MeshComponent.h"
#include <ModuleManager.h>

MeshComponent::MeshComponent(Actor* owner,
	Transform trans,
	vec2* locations,
	vec2* UVs,
	uint32* elems,
	uint32 numVerts,
	uint32 numElems)
	:	SceneComponent(owner, trans),
		model(ModuleManager::get().getRenderer().newModel(locations, UVs, elems, numVerts, numElems))
{
	
}