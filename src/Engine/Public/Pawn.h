#pragma once

#include "Engine.h"

#include "Actor.h"

// forward declarations
class Controller;

class Pawn : public Actor
{
public:
	explicit Pawn(const Transform& trans);
	virtual ~Pawn() override;

private:

	Controller* controller;

};