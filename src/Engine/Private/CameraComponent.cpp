#include "CameraComponent.h"

CameraComponent::CameraComponent(Actor* owner, Transform trans, const mat4& viewMatIn)
	: SceneComponent(owner, trans)
{
	memcpy(viewMat, &viewMatIn[0][0], sizeof(float) * 16);
	
}

CameraComponent::~CameraComponent()
{
	
}

mat4 CameraComponent::getViewMat() const
{
	return mat4(
		viewMat[0],
		viewMat[1],
		viewMat[2],
		viewMat[3],
		viewMat[4],
		viewMat[5],
		viewMat[6],
		viewMat[7],
		viewMat[8],
		viewMat[9],
		viewMat[10],
		viewMat[11],
		viewMat[12],
		viewMat[13],
		viewMat[14],
		viewMat[15]
		);
}

void CameraComponent::setViewMat(const mat4& newMat)
{

	memcpy(viewMat, &newMat[0][0], sizeof(float) * 16);
}