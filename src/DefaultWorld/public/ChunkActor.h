#pragma once

#include "DefaultWorldConfig.h"

#include <Actor.h>
#include <Model.h>
#include <MeshComponent.h>
#include <ModelData.h>

class ChunkActor : public Actor
{
public:
	ChunkActor(const Transform& trans, std::shared_ptr<MaterialInstance> mat, std::shared_ptr<ModelData> data)
		: Actor(),
		meshComp(*this, Transform{}, mat, data, 1)
	{
		setWorldTransform(trans);
	}

private:
	MeshComponent meshComp;
};
