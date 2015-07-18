#pragma once

#include "Engine.h"
#include "Texture.h"
#include "QuadUVCoords.h"

#include <boost/optional.hpp>

#include <string>

class TextureLibrary : public Texture
{
public:

	virtual ~TextureLibrary() = default;

	virtual void addImage(const std::string& name) = 0;

	virtual boost::optional<QuadUVCoords> getUVCoords(const std::string& name) = 0;

};