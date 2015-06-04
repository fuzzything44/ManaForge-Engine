#include "CameraComponent.h"

#include "Helper.h"

CameraComponent::CameraComponent(Actor* owner, Transform trans, float aspectRatio, float zoom)
	: SceneComponent(owner, trans),
	aspectRatio(aspectRatio),
	zoom(zoom)
{
	
}

CameraComponent::~CameraComponent()
{
	
}

void CameraComponent::setAspectRatio(float newAsepctRatio)
{
	aspectRatio = newAsepctRatio;
}

void CameraComponent::setZoom(float newZoom)
{
	zoom = newZoom;
}


float CameraComponent::getAspectRatio() const
{
	return aspectRatio;
}

float CameraComponent::getZoom() const
{
	return zoom;
}


mat4 CameraComponent::getViewMat() const
{
	mat4 ret = glm::ortho(-1.f, 1.f, -aspectRatio, aspectRatio, .1f, 100.f);
	ret = glm::scale(ret, vec3(zoom, zoom, 1.f));
	ret = glm::translate(ret, vec3(trans.location.x, trans.location.y, 0.f));

	ENG_LOGLN(aspectRatio);

	return ret;
}