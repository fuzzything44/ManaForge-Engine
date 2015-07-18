#pragma once
#include "OpenGLRendererConfig.h"

#include <MaterialInstance.h>
#include <Texture.h>

#include <string>
#include <vector>

#include <boost/optional.hpp>

class OpenGLTexture;
class OpenGLMaterialSource;

class OpenGLMaterialInstance : public MaterialInstance
{
public:
	OpenGLMaterialInstance(std::shared_ptr<MaterialSource> source = nullptr);
	virtual ~OpenGLMaterialInstance() override;

	void virtual setTexture(uint32 ID, std::shared_ptr<Texture> texture) override;
	virtual void init(std::shared_ptr<MaterialSource> source) override;

	virtual std::shared_ptr<MaterialSource> getSource() override;
	virtual std::shared_ptr<const MaterialSource> getSource() const override;


	void use();


private:

	const static uint32 maxTextures = 32;

	std::shared_ptr<OpenGLMaterialSource> program;

	// vector of <texture ID>
	std::array< boost::optional<std::shared_ptr<OpenGLTexture> >, maxTextures> textures;
	int32 startTexUniform;
};