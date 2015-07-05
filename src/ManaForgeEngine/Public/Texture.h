#pragma once

#include "Engine.h"
#include <string>



class Texture
{
public:

	explicit Texture(){ }

	virtual ~Texture(){ }

	enum class FilterMode : uint8
	{
		NEAREST,
		LINEAR,
		MIPMAP_NEAREST,
		MIPMAP_LINEAR
	};

	enum class WrapMode : uint8
	{
		CLAMP_TO_EDGE,
		MIRRORED_REPEAT, 
		REPEAT
	};

	virtual uint32 getID() = 0;
	virtual void setFilterMode(FilterMode newMode) = 0;
	virtual FilterMode getFilterMode() const = 0;

	virtual void setWrapMode(WrapMode newMode) = 0;
	virtual WrapMode getWrapMode() const = 0;
};