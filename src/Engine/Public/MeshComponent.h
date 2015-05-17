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
		ModelData data = ModelData{});


protected:

	Model* model;
};