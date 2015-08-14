#pragma once

#include "TestContentConfig.h"

#include <SaveData.h>

#include <Actor.h>
#include <MeshComponent.h>
#include <AudioComponent.h>
#include <PhysicsComponent.h>
#include <TextBox.h>

class Pew;

MFCLASS(Gate, Actor)
class Gate : public Actor, TickingActor<Gate>
{
	MFCLASS_BODY(Gate, isOpen)

	std::unique_ptr<MeshComponent> meshComp;
	std::unique_ptr<PhysicsComponent> physComp;

	std::unique_ptr<MaterialInstance> mat;

	static bool isInitalized;


public:
	explicit Gate();
	virtual ~Gate() override;
	void tick(float) { MFLOG(Debug) << "IT WORKED B0SS"; }

	bool isOpen;
};