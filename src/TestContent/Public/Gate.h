#pragma once

#include "TestContentConfig.h"

#include <SaveData.h>

#include <Actor.h>
#include <MeshComponent.h>
#include <AudioComponent.h>

class Gate : public Actor
{
	INIT_SAVED_CLASS()

		AudioComponent* audioComp;
	MeshComponent* meshComp;

public:

	explicit Gate();

	virtual ~Gate() override;

	bool isOpen;
};

REGISTER_FOR_SAVING_HEADER(Gate, TestContent_ModuleName)
