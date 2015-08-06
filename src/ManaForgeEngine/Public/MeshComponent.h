#pragma once

#include "SceneComponent.h"
#include "Model.h"

class MaterialInstance;

class MeshComponent : public SceneComponent
{
public:
	inline explicit MeshComponent(Actor& owner,
		Transform trans,
		std::shared_ptr<MaterialInstance> mat,
		std::shared_ptr<ModelData> data);

protected:
	std::unique_ptr<Model> model;
};

MeshComponent::MeshComponent(
	Actor& owner, Transform trans, std::shared_ptr<MaterialInstance> mat, std::shared_ptr<ModelData> data)
	: SceneComponent(owner, trans)
	, model(Runtime::get().renderer->newModel())
{
	model->init(mat, data, *this);
}