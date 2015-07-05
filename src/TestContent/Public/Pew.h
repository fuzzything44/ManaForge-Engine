#pragma once
#include "TestContentConfig.h"
#include <Actor.h>

class Pew : public Actor {
public:
	Pew() : timeToSpawn(0.f) { Runtime::get().world->registerTickingActor(*this); }
	~Pew() override {}
	virtual void tick(float deltaTime) override;
private:
	float timeToSpawn;
};
