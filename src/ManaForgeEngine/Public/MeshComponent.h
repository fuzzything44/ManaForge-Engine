#pragma once


#include "SceneComponent.h"
#include "Model.h"

class MaterialInstance;

class MeshComponent : public SceneComponent
{
public:

	ENGINE_API explicit MeshComponent(
		Actor& owner,
		Transform trans,
		MaterialInstance& mat,
		std::shared_ptr<ModelData> data);

	ENGINE_API virtual ~MeshComponent();


protected:

	std::unique_ptr<Model> model;
}; 