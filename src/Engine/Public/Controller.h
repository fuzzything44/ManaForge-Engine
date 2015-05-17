#pragma once

#include "Engine.h"

// forawrd declarations
class Pawn;

class Controller
{
public:
	Controller();
	virtual ~Controller();

private:
	Pawn* controlledPawn;

};