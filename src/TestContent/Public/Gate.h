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

	std::unique_ptr<MeshComponent> meshComp;
	std::unique_ptr<PhysicsComponent> physComp;
	static std::shared_ptr<Material> mat;
	static bool isInitalized;
	float timeToDestruction;
public:

	explicit Gate();
	virtual void tick(float deltaTime) override;
	virtual ~Gate() override;

	bool isOpen;
};

REGISTER_FOR_SAVING_HEADER(Gate)
