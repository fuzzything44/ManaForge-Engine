#pragma once


#include "SceneComponent.h"
#include "Model.h"

class Material;

class MeshComponent : public SceneComponent
{
public:
	explicit ENGINE_API MeshComponent(
		Actor* owner = nullptr,
		Transform trans = Transform{},
		vec2* locations = nullptr,
		vec2* UVs = nullptr,
		uvec3* tris = nullptr,
		uint32 numVerts = 0,
		uint32 numTris = 0,
		Material* material = nullptr);


protected:

	Model* model;
};