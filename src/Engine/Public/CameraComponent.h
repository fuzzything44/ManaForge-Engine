#pragma once

#include "Engine.h"
#include "SceneComponent.h"


class CameraComponent : public SceneComponent
{
public:
	ENGINE_API explicit CameraComponent(Actor* owner = nullptr, Transform trans = Transform{}, const mat4& viewMat = mat4{});
	ENGINE_API virtual ~CameraComponent() override;

	ENGINE_API mat4 getViewMat() const;
	ENGINE_API void setViewMat(const mat4& newMat);

protected:
	
	//mat4* viewMat;
};

