#pragma once

#include "Engine.h"

#include <string>

class Texture;

class Material
{
public:
	Material() {};

	virtual ~Material()
	{
	}


	virtual void setTexture(uint32 ID, Texture& texture) = 0;

	
};