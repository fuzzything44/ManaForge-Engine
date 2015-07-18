#pragma once

#include "DefaultWorldConfig.h"

#include <Actor.h>
#include <Model.h>
#include <MeshComponent.h>
#include <ModelData.h>

class ChunkActor : public Actor
{
public:
	ChunkActor(const Transform& trans, MaterialInstance& mat, std::shared_ptr<ModelData> data)
		:Actor()
	{
		setWorldTransform(trans);

		meshComp = std::make_shared<MeshComponent>(*this, Transform{}, mat, data);
	}

private:

	std::shared_ptr<MeshComponent> meshComp;

};
