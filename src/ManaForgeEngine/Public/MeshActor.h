#include "Engine.h"

#include "Actor.h"
#include "MeshComponent.h"

class MeshActor : public Actor
{
	MeshActor(const Transform& trans = Transform{}, bool isPersistent = false, Model* mesh = nullptr);

	MeshComponent mesh;
};