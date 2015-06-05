#include "CameraComponent.h"

#include "Helper.h"

#include "glm-ortho-2d.h"

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


mat3 CameraComponent::getViewMat() const
{
	mat3 ret = glm::ortho2d(-1.f, 1.f, -aspectRatio, aspectRatio);
	ret = glm::scale(ret, vec2(zoom, zoom));
	ret = glm::rotate(ret, trans.rotation);
	ret = glm::translate(ret, vec2(trans.location.x, trans.location.y));
	

	return ret;
}