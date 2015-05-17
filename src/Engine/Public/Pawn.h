#pragma once

#include "Engine.h"

#include "Actor.h"

class Pawn : public Actor
{
public:
	explicit Pawn(const Transform& trans);
	virtual ~Pawn() override;



};