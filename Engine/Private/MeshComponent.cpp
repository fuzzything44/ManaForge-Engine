#include "MeshComponent.h"
#include <Runtime.h>

MeshComponent::MeshComponent(Actor* owner,
	Transform trans,
	vec2* locations,
	vec2* UVs,
	uvec3* tris,
	uint32 numTris,
	uint32 numElems,
	Material* material)
	:	SceneComponent(owner, trans),
		model(Runtime::get().moduleManager.getRenderer().newModel(locations, UVs, tris, numTris, numElems, material))
{
	
}