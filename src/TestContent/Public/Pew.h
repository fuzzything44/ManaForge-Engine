#pragma once
#include "TestContentConfig.h"
#include <Actor.h>
#include <PhysicsComponent.h>
#include <TextBox.h>

MFCLASS(Pew, Actor)
class Pew : public Actor
{

	MFCLASS_BODY(Pew)
public:
	Pew();
	~Pew() override {}

	TestContent_API void startContact(PhysicsComponent& other);

private:
	std::unique_ptr<PhysicsComponent> phys;
	std::unique_ptr<TextBox> textBox;

	float timeToSpawn;

	int contacts = 0;
};
