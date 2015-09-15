#pragma once

#include "SceneComponent.h"

class MaterialInstance;
class ModelData;
class Model;

class MeshComponent : public SceneComponent
{
public:
	inline explicit MeshComponent(Actor& owner,
		Transform trans,
		std::shared_ptr<MaterialInstance> mat,
		std::shared_ptr<ModelData> data,
		uint8 renderOrder);

protected:
	std::unique_ptr<Model> model;
};

#include "Model.h"
#include "Renderer.h"

inline MeshComponent::MeshComponent(
	Actor& owner, Transform trans, std::shared_ptr<MaterialInstance> mat, std::shared_ptr<ModelData> data, uint8 renderOrder)
	: SceneComponent(owner, trans)
	, model(Runtime::get().getRenderer().newModel(renderOrder))
{
	model->init(mat, data, *this);
}