#pragma once
#include "TestContentConfig.h"
#include <Actor.h>

MFCLASS(Pew, Actor)
class Pew : public Actor, TickingActor<Pew> {

	MFCLASS_BODY(Pew)
public:
	Pew() : timeToSpawn(0.f) { }
	~Pew() override {}
	virtual void tick(float deltaTime) override;
private:
	float timeToSpawn;
};
