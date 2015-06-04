#pragma once

#include "Engine.h"
#include "SceneComponent.h"

#include <glm/gtx/type_aligned.hpp>

class CameraComponent : public SceneComponent
{
public:
	// aspect ratio is (WINDOW Y)/(WINDOW X)
	ENGINE_API explicit CameraComponent(Actor* owner = nullptr, Transform trans = Transform{}, float aspectRatio = 1.f, float zoom = 1.f);
	ENGINE_API virtual ~CameraComponent() override;

	ENGINE_API mat4 getViewMat() const;

	ENGINE_API void setAspectRatio(float newAsepctRatio);
	ENGINE_API void setZoom(float newZoom);

	ENGINE_API float getAspectRatio() const;
	ENGINE_API float getZoom() const;


protected:
	float aspectRatio;
	float zoom;
};

