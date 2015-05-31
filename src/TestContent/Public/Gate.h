#pragma once

#include "TestContentConfig.h"

#include <SaveData.h>

#include <Actor.h>
#include <MeshComponent.h>

class Gate : public Actor
{
	INIT_SAVED_CLASS()

public:

	explicit Gate(const Transform& trans = Transform{});

	virtual ~Gate() override;

	bool isOpen;
};

REGISTER_FOR_SAVING_HEADER(Gate, TestContent_ModuleName)
