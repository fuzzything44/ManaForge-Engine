#pragma once
#include "OpenGLRendererConfig.h"

#include <MaterialInstance.h>
#include <Texture.h>

#include <string>
#include <vector>
#include <unordered_map>

#include <boost/optional.hpp>

class OpenGLTexture;
class OpenGLMaterialSource;
class OpenGLRenderer;

class OpenGLMaterialInstance : public MaterialInstance
{
  public:
	OpenGLMaterialInstance(OpenGLRenderer& renderer, std::shared_ptr<MaterialSource> source = nullptr);
	virtual ~OpenGLMaterialInstance() override;

	void virtual setTexture(uint32 ID, std::shared_ptr<Texture> texture) override;
	virtual void init(std::shared_ptr<MaterialSource> source) override;

	virtual std::shared_ptr<MaterialSource> getSource() override;
	virtual std::shared_ptr<const MaterialSource> getSource() const override;

	virtual void setUpdateCallback(std::function<void(MaterialInstance&)>) override;

	// property interface
	virtual void setProperty(const std::string& propName, int i) override;
	virtual void setProperty(const std::string& propName, const ivec2& i) override;
	virtual void setProperty(const std::string& propName, const ivec3& i) override;
	virtual void setProperty(const std::string& propName, const ivec4& i) override;
	virtual void setProperty(const std::string& propName, int* i, size_t size) override;

	virtual void setProperty(const std::string& propName, float i) override;
	virtual void setProperty(const std::string& propName, const vec2& i) override;
	virtual void setProperty(const std::string& propName, const vec3& i) override;
	virtual void setProperty(const std::string& propName, const vec4& i) override;
	virtual void setProperty(const std::string& propName, float* i, size_t size) override;

	virtual void setPropertyMatrix(const std::string& propName, const mat2& i) override;
	virtual void setPropertyMatrix(const std::string& propName, const mat3& i) override;
	virtual void setPropertyMatrix(const std::string& propName, const mat4& i) override;

	virtual void setPropertyMatrix2ptr(const std::string& propName, float* i) override;
	virtual void setPropertyMatrix3ptr(const std::string& propName, float* i) override;
	virtual void setPropertyMatrix4ptr(const std::string& propName, float* i) override;
	// end property interface

	void use();

  private:
	OpenGLRenderer& renderer;

	const static uint32 maxTextures = 32;

	std::function<void(MaterialInstance&)> updateCallback;

	std::shared_ptr<OpenGLMaterialSource> program;

	std::unordered_map<std::string, std::tuple<int32, std::function<void(int32)>>> properties;

	// vector of <texture ID>
	std::array<boost::optional<std::shared_ptr<OpenGLTexture>>, maxTextures> textures;
};