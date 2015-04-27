#include "MeshComponent.h"
#include <Runtime.h>

MeshComponent::MeshComponent(Actor* owner,
	Transform trans,
	vec2* locations,
	vec2* UVs,
	uint32* elems,
	uint32 numVerts,
	uint32 numElems)
	:	SceneComponent(owner, trans),
		model(Runtime::get().moduleManager.getRenderer().newModel(locations, UVs, elems, numVerts, numElems))
{
	
}