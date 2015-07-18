#pragma once

#include "Engine.h"

#include <string>
#include <memory>

class Texture;
class MaterialSource;

class MaterialInstance
{
public:

	virtual ~MaterialInstance() = default;

	virtual void init(std::shared_ptr<MaterialSource> source) = 0;
	virtual void setTexture(uint32 ID, std::shared_ptr<Texture> texture) = 0;

	virtual std::shared_ptr<MaterialSource> getSource() = 0;	
	virtual std::shared_ptr<const MaterialSource> getSource() const = 0;

	
};