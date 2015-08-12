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
class Gate : public Actor
{
	MFCLASS_BODY(Gate)

	std::unique_ptr<MeshComponent> meshComp;
	std::unique_ptr<PhysicsComponent> physComp;

	std::unique_ptr<MaterialInstance> mat;

	std::unique_ptr<TextBox> textBox;

	static bool isInitalized;

public:
	explicit Gate();
	virtual ~Gate() override;

	bool isOpen;
};