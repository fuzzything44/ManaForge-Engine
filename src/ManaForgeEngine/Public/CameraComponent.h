#pragma once

#include "Engine.h"
#include "SceneComponent.h"
#include "glm-ortho-2d.h"

class CameraComponent : public SceneComponent
{
  public:
	// aspect ratio is (WINDOW Y)/(WINDOW X)
	ENGINE_API inline explicit CameraComponent(
		Actor& owner, Transform trans = Transform{}, float aspectRatio = 1.f, float zoom = 1.f);
	ENGINE_API inline virtual ~CameraComponent() override;

	ENGINE_API inline mat3 getViewMat() const;

	ENGINE_API inline void setAspectRatio(float newAsepctRatio);
	ENGINE_API inline void setZoom(float newZoom);

	ENGINE_API inline float getAspectRatio() const;
	ENGINE_API inline float getZoom() const;

  protected:
	float aspectRatio;
	float zoom;
};

//////////////////////////////////////////////////////////////////////////
/////////// INLINE DEFINITIONS

inline CameraComponent::CameraComponent(Actor& owner, Transform trans, float aspectRatio, float zoom)
	: SceneComponent(owner, trans)
	, aspectRatio(aspectRatio)
	, zoom(zoom)
{
}

inline CameraComponent::~CameraComponent() {}

inline void CameraComponent::setAspectRatio(float newAsepctRatio) { aspectRatio = newAsepctRatio; }

inline void CameraComponent::setZoom(float newZoom) { zoom = newZoom; }

inline float CameraComponent::getAspectRatio() const { return aspectRatio; }

inline float CameraComponent::getZoom() const { return zoom; }

inline mat3 CameraComponent::getViewMat() const
{
	Transform worldTrans = getWorldTransform();

	mat3 ret = glm::ortho2d(-1.f, 1.f, -aspectRatio, aspectRatio);
	ret = glm::scale(ret, vec2(zoom, zoom));
	ret = glm::rotate(ret, -worldTrans.rotation);	// same with below
	ret = glm::translate(ret, -worldTrans.location); // we are translating the camera - so we change the sign

	return ret;
}