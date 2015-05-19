#pragma once

#include "Engine.h"
#include <string>

class Texture
{
public:
	explicit Texture(const std::string& name = std::string()){}

	virtual ~Texture(){ }

	virtual std::string getPath() const = 0;

};