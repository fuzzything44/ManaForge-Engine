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

	virtual void setUpdateCallback(std::function<void(MaterialInstance&)>) = 0;

	virtual void setProperty(const std::string& propName, int i) = 0;
	virtual void setProperty(const std::string& propName, const ivec2& i) = 0;
	virtual void setProperty(const std::string& propName, const ivec3& i) = 0;
	virtual void setProperty(const std::string& propName, const ivec4& i) = 0;
	virtual void setProperty(const std::string& propName, int* i, size_t size) = 0;

	virtual void setProperty(const std::string& propName, float i) = 0;
	virtual void setProperty(const std::string& propName, const vec2& i) = 0;
	virtual void setProperty(const std::string& propName, const vec3& i) = 0;
	virtual void setProperty(const std::string& propName, const vec4& i) = 0;
	virtual void setProperty(const std::string& propName, float* i, size_t size) = 0;

	virtual void setPropertyMatrix(const std::string& propName, const mat2& i) = 0;
	virtual void setPropertyMatrix(const std::string& propName, const mat3& i) = 0;
	virtual void setPropertyMatrix(const std::string& propName, const mat4& i) = 0;

	virtual void setPropertyMatrix2ptr(const std::string& propName, float* i) = 0;
	virtual void setPropertyMatrix3ptr(const std::string& propName, float* i) = 0;
	virtual void setPropertyMatrix4ptr(const std::string& propName, float* i) = 0;
};