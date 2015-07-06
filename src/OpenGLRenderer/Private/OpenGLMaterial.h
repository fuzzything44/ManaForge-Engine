#pragma once
#include "OpenGLRendererConfig.h"

#include <Material.h>
#include <Texture.h>

#include <string>
#include <vector>


class OpenGLMaterial : public Material
{
public:
	OpenGLMaterial(const std::string& name);
	virtual ~OpenGLMaterial() override;

	void addShaderProgramFromFile(std::string filename);


	void virtual setTexture(uint32 ID, Texture& texture) override;

	void use();

	GLint operator()();


private:

	const static uint32 maxTextures = 32;

	GLuint program;

	// vector of <texture ID>
	GLuint textures[maxTextures];
	GLint startTexUniform;
};