#pragma once
#include "OpenGLRendererConfig.h"

#include <Material.h>
#include <Texture.h>

#include <string>
#include <vector>


class OpenGLMaterial : public Material
{
public:
	~OpenGLMaterial() override;

	void virtual addShaderProgramFromFile(std::string filename) override;
	void virtual addShaderProgramFromSource(std::string shader) override;
	void virtual setTexture(uint32 ID, Texture* texture) override;

	void use();

	GLint operator()();


private:

	GLuint program;

	// vector of <texture ID>
	std::vector<GLuint> textures;
	static GLint startTexUniform;
};