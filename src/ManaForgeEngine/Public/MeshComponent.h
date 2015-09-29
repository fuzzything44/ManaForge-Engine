#pragma once

#include "SceneComponent.h"

class MaterialInstance;
class ModelData;

#include "Model.h"

class MeshComponent : public SceneComponent
{
public:
	inline explicit MeshComponent(Actor& owner,
		Transform trans,
		std::shared_ptr<MaterialInstance> mat,
		std::shared_ptr<ModelData> data,
		uint8 renderOrder);

	inline virtual ~MeshComponent();

protected:
	std::unique_ptr<Model, decltype(&Model::deleter)> model;
};

#include "Renderer.h"

inline MeshComponent::MeshComponent(Actor& owner,
	Transform trans,
	std::shared_ptr<MaterialInstance> mat,
	std::shared_ptr<ModelData> data,
	uint8 renderOrder)
	: SceneComponent(owner, trans)
	, model(Runtime::get().getRenderer().newModel(renderOrder))
{
	model->init(mat, data, *this);
}

inline MeshComponent::~MeshComponent() = default;