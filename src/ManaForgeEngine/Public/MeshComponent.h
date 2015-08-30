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
		std::shared_ptr<ModelData> data);

protected:
	std::unique_ptr<Model> model;
};

#include "Model.h"
#include "Renderer.h"

inline MeshComponent::MeshComponent(
	Actor& owner, Transform trans, std::shared_ptr<MaterialInstance> mat, std::shared_ptr<ModelData> data)
	: SceneComponent(owner, trans)
	, model(Runtime::get().getRenderer().newModel())
{
	model->init(mat, data, *this);
}