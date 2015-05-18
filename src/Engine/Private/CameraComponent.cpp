#include "CameraComponent.h"

CameraComponent::CameraComponent(Actor* owner, Transform trans, const mat4& viewMat)
	: SceneComponent(owner, trans),
	viewMat(new mat4(viewMat))
{
}

mat4 CameraComponent::getViewMat() const
{
	return *viewMat;
}

void CameraComponent::setViewMat(const mat4& newMat)
{
	*viewMat = newMat;
}