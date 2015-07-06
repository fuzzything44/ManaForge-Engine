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

		meshComp = std::make_shared<MeshComponent>(*this, Transform{}, data);
	}

private:

	std::shared_ptr<MeshComponent> meshComp;

};
