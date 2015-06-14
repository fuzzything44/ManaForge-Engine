#pragma once

#include "TestContentConfig.h"

#include <SaveData.h>

#include <Actor.h>
#include <MeshComponent.h>
#include <AudioComponent.h>
#include <PhysicsComponent.h>

class Gate : public Actor
{
	INIT_SAVED_CLASS()

	MeshComponent* meshComp;
	PhysicsComponent* physComp;
	Material* mat;
public:

	explicit Gate();

	virtual ~Gate() override;

	bool isOpen;
};

REGISTER_FOR_SAVING_HEADER(Gate, TestContent_ModuleName)
