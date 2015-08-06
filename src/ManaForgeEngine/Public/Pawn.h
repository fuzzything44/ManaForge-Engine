#pragma once

#include "Engine.h"

#include "Actor.h"

// forward declarations
class Controller;

class Pawn : public Actor
{
public:
	ENGINE_API explicit Pawn();
	ENGINE_API virtual ~Pawn() override;

private:
	Controller* controller;
};