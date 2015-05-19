#pragma once

#include "Engine.h"

#include <string>

class Texture;

class Material
{
public:
	virtual ~Material()
	{
	}

	Material() {} ;

	virtual void addShaderProgramFromFile(std::string filename) = 0;
	virtual void addShaderProgramFromSource(std::string shader) = 0;

	virtual void setTexture(uint32 ID, Texture* texture) = 0;

	
};