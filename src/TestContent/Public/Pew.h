#pragma once
#include "TestContentConfig.h"
#include <Actor.h>
#include <PhysicsComponent.h>

MFCLASS(Pew, Actor)
class Pew : public Actor
{

	MFCLASS_BODY(Pew)
  public:
	Pew();
	~Pew() override {}

	void startContact(PhysicsComponent& other);

  private:
	std::unique_ptr<PhysicsComponent> phys;

	float timeToSpawn;
};
