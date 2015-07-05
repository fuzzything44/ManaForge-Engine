#pragma once

#include "Engine.h"

// forawrd declarations
class Pawn;

class Controller
{
public:
	ENGINE_API Controller();
	ENGINE_API virtual ~Controller();

private:
	Pawn* controlledPawn;

};