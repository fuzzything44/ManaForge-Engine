#include "CameraComponent.h"

CameraComponent::CameraComponent(Actor* owner, Transform trans, const mat4& viewMat)
	: SceneComponent(owner, trans),
	viewMat(viewMat)
{
}

mat4 CameraComponent::getViewMat()
{
	return viewMat;
}