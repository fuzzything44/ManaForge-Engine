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

	virtual uint32 getID() = 0;
	virtual void setFilterMode(FilterMode mode) = 0;
	virtual FilterMode getFilterMode() const = 0;



};