#pragma once


#include "SceneComponent.h"
#include "Model.h"

class MeshComponent : public SceneComponent
{
	explicit ENGINE_API MeshComponent(
		Actor* owner = nullptr,
		Transform trans = Transform{},
		vec2* locations = nullptr,
		vec2* UVs = nullptr,
		uint32* elems = nullptr,
		uint32 numVerts = 0,
		uint32 numElems = 0);

private:

	Model* model;
};