#pragma once

#include "TestContentConfig.h"

#include <SaveData.h>

#include <Actor.h>
#include <MeshComponent.h>
#include <AudioComponent.h>
#include <PhysicsComponent.h>

class Gate : public Actor, TickingActor<Gate>
{
	INIT_SAVED_CLASS()

	MeshComponent* meshComp;
	PhysicsComponent* physComp;
	std::shared_ptr<Material> mat;
	float timeToDestruction;
public:

	explicit Gate();
	virtual void tick(float deltaTime) override;
	virtual ~Gate() override;

	bool isOpen;
};

REGISTER_FOR_SAVING_HEADER(Gate)
