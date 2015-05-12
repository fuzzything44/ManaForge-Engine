#pragma once
#include "OpenGLRendererConfig.h"

#include <Material.h>

#include <string>
#include <vector>

class OpenGLMaterial : public Material
{
public:
	~OpenGLMaterial() override;
	void addShaderProgramFromFile(std::string filename) override;
	void addShaderProgramFromSource(std::string shader) override;
	void setTexture(uint32 ID, std::string texture) override;

	void use();

	int32 getUniformLocation(const char* name);

private:

	GLuint program;

	// vector of <texture ID>
	std::vector<GLuint> textures;
	static GLint startTexUniform;
};