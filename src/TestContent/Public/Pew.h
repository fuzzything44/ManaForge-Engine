#pragma once
#include "TestContentConfig.h"
#include <Actor.h>
#include <PhysicsComponent.h>

MFCLASS(Pew, Actor)
class Pew : public Actor, TickingActor<Pew> {

	MFCLASS_BODY(Pew)
public:
	Pew();
	~Pew() override {}
	virtual void tick(float deltaTime) override;

	void startContact(PhysicsComponent& other);
private:
	std::unique_ptr<PhysicsComponent> phys;

	float timeToSpawn;
};
