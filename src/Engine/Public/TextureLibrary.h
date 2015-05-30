#pragma once

#include "Engine.h"
#include "Texture.h"

#include <string>

class TextureLibrary : public Texture
{
public:

	TextureLibrary() { }

	virtual ~TextureLibrary() { };

	virtual void addImage(const std::string& name) = 0;

};