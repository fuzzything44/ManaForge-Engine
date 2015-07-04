#pragma once


#include "SceneComponent.h"
#include "Model.h"

class Material;

class MeshComponent : public SceneComponent
{
public:

	ENGINE_API explicit MeshComponent(
		Actor& owner,
		Transform trans,
		ModelData data);

	ENGINE_API virtual ~MeshComponent();


protected:

	std::unique_ptr<Model> model;
}; 