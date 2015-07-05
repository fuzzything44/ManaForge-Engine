#include "EnginePCH.h"

#include "MeshComponent.h"

#include "Runtime.h"
#include "Window.h"
#include "Renderer.h"

MeshComponent::MeshComponent(Actor& owner,
	Transform trans,
	ModelData data)
	:	SceneComponent(owner, trans),
		model(Runtime::get().moduleManager.getRenderer().newModel(data, this))
{
	
}

MeshComponent::~MeshComponent()
{
}