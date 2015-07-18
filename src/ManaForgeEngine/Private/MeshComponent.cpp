#include "EnginePCH.h"

#include "MeshComponent.h"

#include "Runtime.h"
#include "Window.h"
#include "Renderer.h"

MeshComponent::MeshComponent(Actor& owner,
	Transform trans,
	MaterialInstance& mat, 
	const std::shared_ptr<ModelData> data)
	:	SceneComponent(owner, trans),
		model(Runtime::get().moduleManager.getRenderer().newModel())
{
	model->init(mat, data, *this);
}

MeshComponent::~MeshComponent()
{
}