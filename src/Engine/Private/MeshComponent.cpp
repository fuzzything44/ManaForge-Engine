#include "MeshComponent.h"
#include <Runtime.h>

MeshComponent::MeshComponent(Actor* owner,
	Transform trans,
	ModelData data)
	:	SceneComponent(owner, trans),
		model(Runtime::get().moduleManager.getRenderer().newModel(data, this))
{
	
}

MeshComponent::~MeshComponent()
{
	//Runtime::get().moduleManager.getRenderer().removeModel(model);
	delete model;
}