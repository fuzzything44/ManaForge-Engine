#pragma once

#include "DefaultWorldConfig.h"

#include <Actor.h>
#include <Model.h>
#include <MeshComponent.h>

class ChunkActor : public Actor
{
public:
	ChunkActor(const Transform& trans, const ModelData& data)
		:Actor()
	{
		setWorldTransform(trans);

		meshComp = new MeshComponent(*this, Transform{}, data);
	}

private:

	MeshComponent* meshComp;

};
