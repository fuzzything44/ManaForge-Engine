#pragma once

#include "Engine.h"
#include "SceneComponent.h"


class CameraComponent : public SceneComponent
{
public:
	explicit ENGINE_API CameraComponent(Actor* owner = nullptr, Transform trans = Transform{}, const mat4& viewMat = mat4{});

	ENGINE_API mat4 getViewMat();

protected:
	
	mat4 viewMat;
};

