#pragma once

#include "Engine.h"
#include <string>

class Texture
{
public:
	explicit Texture(){ }

	virtual uint32 getID() = 0;

	virtual ~Texture(){ }

};