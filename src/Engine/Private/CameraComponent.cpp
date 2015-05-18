#include "CameraComponent.h"

CameraComponent::CameraComponent(Actor* owner, Transform trans, const mat4& viewMatIn)
	: SceneComponent(owner, trans)
{
	//viewMat = static_cast<mat4*>(_aligned_malloc(sizeof(vec4), 16));
	//*viewMat = viewMatIn;
}

CameraComponent::~CameraComponent()
{
	//_aligned_free(viewMat);
}

mat4 CameraComponent::getViewMat() const
{
	//return *viewMat;
	return glm::ortho(-10.f, 10.f, -10.f, 10.f, .1f, 100.f);
}

void CameraComponent::setViewMat(const mat4& newMat)
{
	//*viewMat = newMat;
}