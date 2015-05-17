#pragma once

#include "TestContentConfig.h"

#include <SaveData.h>

#include <Actor.h>
#include <MeshComponent.h>

class Gate : public Actor
{
	INIT_SAVED_ACTOR()

public:

	explicit Gate(const Transform& trans = Transform{});

	MeshComponent* gateMesh;

	bool isOpen;
};

ACTOR_REGISTER_FOR_SAVING_HEADER(Gate, TestContent_ModuleName)