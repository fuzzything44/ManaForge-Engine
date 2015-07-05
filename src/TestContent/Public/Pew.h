#pragma once
#include "TestContentConfig.h"
#include <Actor.h>

class Pew : public Actor, TickingActor<Pew> {
public:
	Pew() : timeToSpawn(0.f) { }
	~Pew() override {}
	virtual void tick(float deltaTime) override;
private:
	float timeToSpawn;
};
