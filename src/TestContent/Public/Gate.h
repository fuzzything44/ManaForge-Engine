#pragma once

#include "TestContentConfig.h"

#include <SaveData.h>

#include <Actor.h>
#include <MeshComponent.h>
#include <AudioComponent.h>
#include <PhysicsComponent.h>

class Pew;

MFCLASS(Gate, Actor)
class Gate : public Actor, TickingActor<Gate>
{
	MFCLASS_BODY(Gate)

	std::unique_ptr<MeshComponent> meshComp;
	std::unique_ptr<PhysicsComponent> physComp;

	std::unique_ptr<MaterialInstance> mat;
	static bool isInitalized;
	float timeToDestruction;

public:

	explicit Gate();
	virtual void tick(float deltaTime) override;
	virtual ~Gate() override;

	bool isOpen;
};